#include <fstream>
#include <cmath>
#include <sdl_gfxprimitives.h>
#include "globe.h"
#include "uibutton.h"
#include "util.h"
#include "uimanager.h"
#include "uiedit.h"

namespace ufo
{
	using namespace Point;

	class NewBaseNameEdit : public UIEdit
	{
		BigFont m_font;
		GeoPoint m_location;

	public:

		NewBaseNameEdit(Sint16 _x, Sint16 _y, GeoPoint location)
			: UIEdit(m_font, _x, _y, 140, 13, true), m_location(location) { }

		void onEnter()
		{
			m_ui->state.bases.push_back(shared_ptr<Base>(new Base));
			m_ui->state.bases.back()->getLocation() = m_location;
			m_ui->state.bases.back()->setName(m_buffer);
			m_ui->destroy(m_parent);
		}
	};

	class NewBaseNameDialog : public UIDialog
	{
		GeoPoint m_location;

	public:

		NewBaseNameDialog(GeoPoint location)
			: UIDialog(32, 60, 192, 80, Palette::blockSize * 8 + 6, UIDialog::Both), m_location(location) { }

		void onCreate()
		{
			m_bg.loadSCR("geograph/back01.scr");

			Palette backPalette("geodata/backpals.dat", 0, 16);
			backPalette.apply(m_bg);
		}

		void onOpen()
		{
			m_parent->create(new NewBaseNameEdit(x + 22, y + 50, m_location));
		}

		void draw(Surface& surface)
		{
			UIDialog::draw(surface);

			Rect r(x, y + 16, w, h);
			m_ui->text.setColor(Palette::blockSize * 8 + 6);
			m_ui->text.print(surface, r, 145, TextRenderer::BigFont, TextRenderer::AlignCenter);
		}
	};

	class NewBaseDialogButton : public UIPushButtonStandard
	{
	public:

		NewBaseDialogButton()
			: UIPushButtonStandard(49, 186, 8, 54, 12) { }

		void onPress()
		{
			m_ui->destroy(m_parent);
		}
	};

	class NewBaseDialog : public UIDialog
	{
		bool m_showCancel;

	public:

		NewBaseDialog(bool showCancel = false)
			: UIDialog(0, 0, 256, 28, Palette::blockSize * 15), m_showCancel(showCancel) { }

		void onCreate()
		{
			m_exclusive = false;

			m_bg.loadSCR("geograph/back01.scr");

			Palette backPalette("geodata/backpals.dat", 0, 16);
			backPalette.apply(m_bg);
			backPalette.apply(m_ui->surface);

			// create dummy button to block geoscape buttons on the right
			create(new UIPushButton(256, 0, 64, 154));

			if (m_showCancel)
				create(new NewBaseDialogButton());

			m_ui->state.time.pause();
		}

		void onDestroy()
		{
			m_ui->state.time.pause(false);
		}

		void draw(Surface& surface)
		{
			UIDialog::draw(surface);
			m_ui->text.setColor(Palette::blockSize * 15);
			m_ui->text.print(surface, Rect(8, 10, w, h), 283, TextRenderer::SmallFont);
		}
	};

	GlobeState::GlobeState()
		: rx(0), rz(720), radius(90)
	{
	}

	void GlobeState::save(string path)
	{
	}

	void GlobeState::load(string path)
	{
		string filename(path + "lease.dat");
		ifstream file(filename.c_str());
		if (!file)
			throw runtime_error("error opening " + filename);

		file.read((char*)&rx, 2);

		file.seekg(6, ios::beg);
		file.read((char*)&rz, 2);

		file.seekg(12, ios::beg);
		file.read((char*)&radius, 2);

		rz = (2880 - rz + 720) % 2880;
		rx = (2880 - rx) % 1440;
	}

	bool GeoPolygon::contains(const Screen& p)
	{
		bool lastResult;
		for (size_t i = 0; i < size(); ++i)
		{
			size_t j = (i + 1) % size();

			if (!at(i).visible || !at(j).visible)
				return false;

			bool result = (p.y - at(i).p.y) * (at(j).p.x - at(i).p.x) - (p.x - at(i).p.x) * (at(j).p.y - at(i).p.y) >= 0;
			if (i == 0)
				lastResult = result;
			else if (result != lastResult)
				return false;
		}

		return true;
	}

	Globe::Globe(StartMode mode)
		: m_polarDegFix(120), UIElement(0, 0, 256, 200), m_palette("geodata/palettes.dat"), m_mouse(0, 0), m_debug(false), m_mode(mode)
	{
	}

	void Globe::onCreate()
	{
		// set up zoom levels
		m_zoomLevels.push_back(90);
		m_zoomLevels.push_back(120);
		m_zoomLevels.push_back(230);
		m_zoomLevels.push_back(360);
		m_zoomLevels.push_back(480);
		m_zoomLevels.push_back(720);

		m_zoom = find(m_zoomLevels.begin(), m_zoomLevels.end(), m_ui->state.globe.radius);

		// set center of globe
		m_center = Screen(w / 2, h / 2);

		// load globe textures
		Surface texture;
		texture.loadSCR("geograph/texture.dat", 32);
		m_palette.apply(texture);

		// set texture frames (each texture is 32x32)
		texture.getFrames(m_textures, 32, 32, 39);

		const string filename("geodata/world.dat");
		ifstream file(filename.c_str(), ios::binary);
		if (!file)
			throw runtime_error("error opening " + filename);

		while (1)
		{
			GeoPoint gp;
			GeoPolygon polygon;
			for (char i = 0; i < 4; ++i)
			{
				file.read((char*)&gp.s.lon, 2);
				if (file.eof())
					break;

				file.read((char*)&gp.s.lat, 2);

				if (gp.s.lon == -1)
					break;

				gp.s.lat += 720;

				polygon.push_back(gp);
			}

			if (file.eof())
				break;

			file.read((char*)&polygon.texture, 4);
			m_polygons.push_back(polygon);
		}

		// update cached cartesian/screen coords
		update();

		if (m_mode == CreateBase)
		{
			m_newBaseDialog = new NewBaseDialog(m_ui->state.bases.size() > 0);
			create(m_newBaseDialog);
		}
	}

	void Globe::logic()
	{
		for (Uint32 i = 0; i < m_test.size(); ++i)
		{
			// move point toward target
			if (SDL_GetTicks() - m_test[i].lastUpdate > 20)
			{
				if (m_test[i].s.lon != m_test[i].target.lon || m_test[i].s.lat != m_test[i].target.lat)
				{
					Spherical targetTemp(m_test[i].target);
					if (m_test[i].s.lat > m_polarDegFix && m_test[i].s.lat < (1440 - m_polarDegFix) && (m_test[i].target.lat <= m_polarDegFix || m_test[i].target.lat >= (1440 - m_polarDegFix)))
						targetTemp.lon = m_test[i].s.lon;

					double direction = atan2(static_cast<double>(targetTemp.lat - m_test[i].s.lat), static_cast<double>(targetTemp.lon - m_test[i].s.lon));
					m_test[i].s.lon += round<Sint16>(cos(direction));
					m_test[i].s.lat += round<Sint16>(sin(direction));
				}

				m_test[i].sync(m_center, m_ui->state.globe.radius, m_ui->state.globe.rx, m_ui->state.globe.rz);

				m_test[i].lastUpdate = SDL_GetTicks();
			}
		}
	}

	void Globe::draw(Surface& surface)
	{
		// set clipping rectangle
		surface.setClipRect(*this);

		// draw ocean
		filledCircleColor(surface.get(), m_center.x, m_center.y, m_ui->state.globe.radius - 1, m_palette.getRGBA(Palette::blockSize * 12));

		// fix clipping rectangle for texturing
		surface.setClipRect(Rect(x, y, w + 1, h));

		vector<Sint16> vx(4), vy(4);
		for (size_t i = 0; i < m_polygons.size(); ++i)
		{
			Sint8 hidden = 0;
			Sint16* minx = &vx[0];
			Sint16* miny = &vy[0];
			Sint16* maxx = &vx[0];
			Sint16* maxy = &vy[0];
			for (size_t j = 0; j < m_polygons[i].size(); ++j)
			{
				if (!m_polygons[i][j].visible)
					++hidden;

				vx[j] = m_polygons[i][j].p.x;
				vy[j] = m_polygons[i][j].p.y;

				// find min x/y and max x/y
				if (vx[j] < *minx)
					minx = &vx[j];
				if (vy[j] < *miny)
					miny = &vy[j];
				if (vx[j] > *maxx)
					maxx = &vx[j];
				if (vy[j] > *maxy)
					maxy = &vy[j];
			}

			// skip polygon if all points are on back of sphere
			if (hidden == m_polygons[i].size())
				continue;

			// slightly enlarge polygons to remove gaps
			(*minx) -= 1;
			if (miny != minx)
				(*miny) -= 1;
			if (maxx != minx && maxx != miny)
				(*maxx) += 1;
			if (maxy != minx && maxy != miny && maxy != maxx)
				(*maxy) += 1;

			if (m_polygons[i].texture == 100)
				filledPolygonColor(surface.get(), &vx[0], &vy[0], m_polygons[i].size(), m_palette.getRGBA(11));
			else
				texturedPolygon(surface.get(), &vx[0], &vy[0], m_polygons[i].size(), m_textures[m_polygons[i].texture + 13 * ((5 - (m_zoom - m_zoomLevels.begin())) / 2)]->get(), 0, 0);
		}

		// set clipping rectangle
		surface.setClipRect(*this);

		// draw bases
		for (Uint32 i = 0; i < m_ui->state.bases.size(); ++i)
			m_ui->state.bases[i]->draw(surface);

		for (Uint32 i = 0; i < m_test.size(); ++i)
		{
			if (m_test[i].visible)
				drawShip(surface, m_test[i].p.x, m_test[i].p.y, 11);
		}

		if (m_defaultTarget.visible)
			drawShip(surface, m_defaultTarget.p.x, m_defaultTarget.p.y, 13);

		if (m_debug)
		{
			m_font.print(surface, 5, 5, format("Rotation (x, z): %d, %d", m_ui->state.globe.rx, m_ui->state.globe.rz));

			GeoPoint gptemp;
			gptemp.c = Cartesian(m_mouse, m_center, m_ui->state.globe.radius, m_ui->state.globe.rx, m_ui->state.globe.rz);
			if (gptemp.c)
			{
				gptemp.s = Spherical(gptemp.c, m_ui->state.globe.radius);
				m_font.print(surface, 5, 15, format("Mouse -> Spherical: %d, %d", gptemp.s.lon, gptemp.s.lat));
				m_font.print(surface, 5, 25, format("Mouse -> Cartesian: %f, %f, %f", gptemp.c.x, gptemp.c.y, gptemp.c.z));
			}

			m_font.print(surface, 5, 45, format("Radius: %d", m_ui->state.globe.radius));
			m_font.print(surface, 5, 35, format("Default Target (Spherical): %d, %d", m_defaultTarget.s.lon, m_defaultTarget.s.lat));

			m_font.print(surface, 5, 55, format("Pixel: %d", surface.getPixel8(m_mouse.x, m_mouse.y)));
		}

		surface.clearClipRect();
	}

	void Globe::drawShip(Surface& surface, Sint16 x, Sint16 y, Uint8 color)
	{
		surface.pixelColor8(x + 1, y, color);
		surface.pixelColor8(x - 1, y, color);
		surface.pixelColor8(x, y + 1, color);
		surface.pixelColor8(x, y - 1, color);
	}

	vector<GeoPolygon>::iterator Globe::screenToPolygon(const Screen& p)
	{
		for (size_t i = 0; i < m_polygons.size(); ++i)
		{
			if (m_polygons[i].contains(p))
			{
				if (m_debug)
					m_polygons[i].texture = 100;
				return m_polygons.begin() + i;
			}
		}

		return m_polygons.end();
	}

	void Globe::update()
	{
		for (size_t i = 0; i < m_polygons.size(); ++i)
		{
			for (size_t j = 0; j < m_polygons[i].size(); ++j)
				m_polygons[i][j].sync(m_center, m_ui->state.globe.radius, m_ui->state.globe.rx, m_ui->state.globe.rz);
		}

		for (size_t i = 0; i < m_test.size(); ++i)
			m_test[i].sync(m_center, m_ui->state.globe.radius, m_ui->state.globe.rx, m_ui->state.globe.rz);

		for (size_t i = 0; i < m_ui->state.bases.size(); ++i)
			m_ui->state.bases[i]->getLocation().sync(m_center, m_ui->state.globe.radius, m_ui->state.globe.rx, m_ui->state.globe.rz);

		m_defaultTarget.sync(m_center, m_ui->state.globe.radius, m_ui->state.globe.rx, m_ui->state.globe.rz);
	}

	void Globe::rotateHorizontal(Sint16 delta)
	{
		m_ui->state.globe.rz = (m_ui->state.globe.rz + delta) % 2880;
		update();
	}

	void Globe::rotateVertical(Sint16 delta)
	{
		m_ui->state.globe.rx = (m_ui->state.globe.rx + delta) % 2880;
		update();
	}

	void Globe::zoom(Sint8 delta)
	{
		if ( (m_zoom != m_zoomLevels.begin() || delta > 0) && (m_zoom + delta != m_zoomLevels.end() || delta < 0) )
		{
			m_zoom += delta;
			m_ui->state.globe.radius = *m_zoom;

			update();
		}
	}

	void Globe::setDefaultTarget(Sint16 sx, Sint16 sy)
	{
		Cartesian p(Screen(sx, sy), m_center, m_ui->state.globe.radius, m_ui->state.globe.rx, m_ui->state.globe.rz);
		if (!p)
			return;

		m_defaultTarget.s = Spherical(p, m_ui->state.globe.radius);

		m_defaultTarget.sync(m_center, m_ui->state.globe.radius, m_ui->state.globe.rx, m_ui->state.globe.rz);
	}

	bool Globe::onMouseLeftClick(Sint16 _x, Sint16 _y)
	{
		if (m_mode == CreateBase)
		{
			vector<GeoPolygon>::iterator i = screenToPolygon(Screen(_x, _y));

			GeoPoint p;
			p.c = Cartesian(Screen(_x, _y), m_center, m_ui->state.globe.radius, m_ui->state.globe.rx, m_ui->state.globe.rz);
			if (p.c && i != m_polygons.end())
			{
				p.s = Spherical(p.c, m_ui->state.globe.radius);
				p.sync(m_center, m_ui->state.globe.radius, m_ui->state.globe.rx, m_ui->state.globe.rz);

				m_mode = Normal;
				m_newBaseDialog->create(new NewBaseNameDialog(p));
			}
		}
		else
		{
			GeoObject gp;
			gp.c = Cartesian(Screen(_x, _y), m_center, m_ui->state.globe.radius, m_ui->state.globe.rx, m_ui->state.globe.rz);
			if (gp.c)
			{
				// populate spherical coordinates
				gp.s = Spherical(gp.c, m_ui->state.globe.radius);
				gp.sync(m_center, m_ui->state.globe.radius, m_ui->state.globe.rx, m_ui->state.globe.rz);

				// set target
				gp.target = m_defaultTarget.s;

				// adjust target if needed
				gp.target.adjust(gp.s);

				gp.lastUpdate = SDL_GetTicks();
				m_test.push_back(gp);
			}
		}

		return true;
	}

	bool Globe::onMouseRightClick(Sint16 x, Sint16 y)
	{
		GeoPoint gp;
		gp.c = Cartesian(Screen(x, y), m_center, m_ui->state.globe.radius, m_ui->state.globe.rx, m_ui->state.globe.rz);
		if (gp.c)
		{
			gp.s = Spherical(gp.c, m_ui->state.globe.radius);

			m_ui->state.globe.rx = gp.s.lat - 720;
			m_ui->state.globe.rz = gp.s.lon + 720;

			update();
		}

		return true;
	}

	bool Globe::onMouseMove(Sint16 x, Sint16 y)
	{
		m_mouse.x = x;
		m_mouse.y = y;

		return true;
	}

	bool Globe::onKeyDown(SDL_keysym keysym)
	{
		if (keysym.sym == SDLK_F12)
			m_debug = !m_debug;
		if (keysym.sym == SDLK_SPACE)
			setDefaultTarget(m_mouse.x, m_mouse.y);

		return false;
	}
}