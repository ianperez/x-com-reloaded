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
			m_ui->state.bases.back()->setLocation(m_location);
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
		: rotx(0), rotz(720), radius(90)
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

		file.read((char*)&rotx, 2);

		file.seekg(6, ios::beg);
		file.read((char*)&rotz, 2);

		file.seekg(12, ios::beg);
		file.read((char*)&radius, 2);

		rotz = (2880 - rotz + 720) % 2880;
		rotx = (2880 - rotx) % 1440;
	}

	Globe::Globe(StartMode mode)
		: m_polarDegFix(120), UIElement(0, 0, 256, 200), m_palette("geodata/palettes.dat"), m_mouse(0, 0), m_debug(false), m_mode(mode)
	{
	}

	void Globe::onCreate()
	{
		m_ui->state.load(1);
		// set up zoom levels
		m_zoomLevels.push_back(90);
		m_zoomLevels.push_back(120);
		m_zoomLevels.push_back(230);
		m_zoomLevels.push_back(360);
		m_zoomLevels.push_back(480);
		m_zoomLevels.push_back(720);

		m_zoom = find(m_zoomLevels.begin(), m_zoomLevels.end(), m_ui->state.globe.radius);

		// set center of globe
		m_center = Point2d(w / 2, h / 2);

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
				file.read((char*)&gp.s.x, 2);
				if (file.eof())
					break;

				file.read((char*)&gp.s.y, 2);

				if (gp.s.x == -1)
					break;

				gp.s.y += 720;

				gp.s.toCartesian(gp.c, m_ui->state.globe.radius);

				polygon.push_back(gp);
			}

			if (file.eof())
				break;

			file.read((char*)&polygon.texture, 4);
			m_polygons.push_back(polygon);
		}

		m_defaultTarget.x = 0;
		m_defaultTarget.y = 0;

		// re-populate object cartesian coordinates
		for (size_t i = 0; i < m_ui->state.bases.size(); ++i)
			m_ui->state.bases[i]->getLocation().sync(m_ui->state.globe.radius);

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
				if (m_test[i].s.x != m_test[i].target.x || m_test[i].s.y != m_test[i].target.y)
				{
					Point2d targetTemp(m_test[i].target);
					if (m_test[i].s.y > m_polarDegFix && m_test[i].s.y < (1440 - m_polarDegFix) && (m_test[i].target.y <= m_polarDegFix || m_test[i].target.y >= (1440 - m_polarDegFix)))
						targetTemp.x = m_test[i].s.x;

					double direction = atan2(static_cast<double>(targetTemp.y - m_test[i].s.y), static_cast<double>(targetTemp.x - m_test[i].s.x));
					m_test[i].s.x += round<Sint16>(cos(direction));
					m_test[i].s.y += round<Sint16>(sin(direction));

					m_test[i].s.toCartesian(m_test[i].c, m_ui->state.globe.radius);
				}

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
			size_t hidden = 0;
			Sint16* minx = &vx[0];
			Sint16* miny = &vy[0];
			Sint16* maxx = &vx[0];
			Sint16* maxy = &vy[0];
			for (size_t j = 0; j < m_polygons[i].size(); ++j)
			{
				Point3d p1(m_polygons[i][j].c);

				// perform rotation
				rotate(p1, m_ui->state.globe.rotx, m_ui->state.globe.rotz);

				// check if point is on back of sphere
				if (p1.y > 0)
					++hidden;

				Point2d p2;
				project(p1, p2);

				vx[j] = p2.x;
				vy[j] = p2.y;

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
		{
			Point3d p1(m_ui->state.bases[i]->getLocation().c);
			rotate(p1, m_ui->state.globe.rotx, m_ui->state.globe.rotz);
			if (p1.y > 0)
				continue;

			Point2d p2;
			project(p1, p2);

			m_ui->state.bases[i]->draw(surface, p2);
		}

		for (Uint32 i = 0; i < m_test.size(); ++i)
		{
			Point3d p1(m_test[i].c);
			rotate(p1, m_ui->state.globe.rotx, m_ui->state.globe.rotz);
			if (p1.y > 0)
				continue;

			Point2d p2;
			project(p1, p2);

			drawShip(surface, p2.x, p2.y, 11);
		}

		Point3d p1;
		m_defaultTarget.toCartesian(p1, m_ui->state.globe.radius);

		rotate(p1, m_ui->state.globe.rotx, m_ui->state.globe.rotz);
		if (p1.y <= 0)
		{
			Point2d p2;
			project(p1, p2);

			drawShip(surface, p2.x, p2.y, 13);
		}

		if (m_debug)
		{
			m_font.print(surface, 5, 5, format("Rotation (x, z): %d, %d", m_ui->state.globe.rotx, m_ui->state.globe.rotz));

			GeoPoint gptemp;
			if (screenToCartesian(m_mouse.x, m_mouse.y, gptemp.c))
			{
				gptemp.c.toSpherical(gptemp.s, m_ui->state.globe.radius);
				m_font.print(surface, 5, 15, format("Mouse -> Spherical: %d, %d", gptemp.s.x, gptemp.s.y));
				m_font.print(surface, 5, 25, format("Mouse -> Cartesian: %f, %f, %f", gptemp.c.x, gptemp.c.y, gptemp.c.z));
			}

			m_font.print(surface, 5, 45, format("Radius: %d", m_ui->state.globe.radius));
			m_font.print(surface, 5, 35, format("Default Target (Spherical): %d, %d", m_defaultTarget.x, m_defaultTarget.y));

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

	// convert Screen coordinates to Cartesian (x,y,z)
	bool Globe::screenToCartesian(Sint16 x, Sint16 y, Point3d& p)
	{
		// unproject point
		p.x = x - m_center.x;
		p.z = -(y - m_center.y);

		double ys = m_ui->state.globe.radius * m_ui->state.globe.radius - p.x * p.x - p.z * p.z;
		if (ys < 0)
			return false;

		p.y = -sqrt(ys);

		// reverse rotation
		rotate(p, -m_ui->state.globe.rotx, 0);
		rotate(p, 0, -m_ui->state.globe.rotz);

		return true;
	}

	vector<GeoPolygon>::iterator Globe::screenToPolygon(Sint16 _x, Sint16 _y)
	{
		for (size_t i = 0; i < m_polygons.size(); ++i)
		{
			bool result = false;
			for (size_t j = 0; j < m_polygons[i].size(); ++j)
			{
				size_t k = (j + 1) % m_polygons[i].size();

				Point3d p1(m_polygons[i][j].c);
				Point3d p2(m_polygons[i][k].c);

				// perform rotation
				rotate(p1, m_ui->state.globe.rotx, m_ui->state.globe.rotz);
				rotate(p2, m_ui->state.globe.rotx, m_ui->state.globe.rotz);

				// check if point is on back of sphere
				if (p1.y > 0 || p2.y > 0)
				{
					result = false;
					break;
				}

				Point2d p3;
				project(p1, p3);

				Point2d p4;
				project(p2, p4);

				if ( ( (p3.y <= _y && _y <= p4.y) || (p4.y <= _y && _y <= p3.y) ) && (_x <= (p4.x - p3.x) * (_y - p3.y) / (p4.y - p3.y) + p3.x) )
					result = !result;
			}

			if (result)
			{
				m_polygons[i].texture = 100;
				return m_polygons.begin() + i;
			}
		}

		return m_polygons.end();
	}

	void Globe::rotate(Point3d& p, Sint16 x, Sint16 z)
	{
		Point3d orig;

		// rotate z-axis
		orig = p;
		p.y = cos(toRad(z)) * orig.y - sin(toRad(z)) * orig.x;
		p.x = sin(toRad(z)) * orig.y + cos(toRad(z)) * orig.x;

		// rotate x-axis
		orig = p;
		p.z = cos(toRad(x)) * orig.z - sin(toRad(x)) * orig.y;
		p.y = sin(toRad(x)) * orig.z + cos(toRad(x)) * orig.y;
	}

	void Globe::project(const Point3d& p1, Point2d& p2)
	{
		p2.x = round<Sint16>(p1.x + m_center.x);
		p2.y = round<Sint16>(-p1.z + m_center.y);
	}

	void Globe::rotateHorizontal(Sint16 delta)
	{
		m_ui->state.globe.rotz = (m_ui->state.globe.rotz + delta) % 2880;
	}

	void Globe::rotateVertical(Sint16 delta)
	{
		m_ui->state.globe.rotx = (m_ui->state.globe.rotx + delta) % 2880;
	}

	void Globe::zoom(Sint8 delta)
	{
		if ( (m_zoom != m_zoomLevels.begin() || delta > 0) && (m_zoom + delta != m_zoomLevels.end() || delta < 0) )
		{
			m_zoom += delta;
			m_ui->state.globe.radius = *m_zoom;

			for (Uint32 i = 0; i < m_polygons.size(); ++i)
			{
				for (Uint32 j = 0; j < m_polygons[i].size(); ++j)
					m_polygons[i][j].sync(m_ui->state.globe.radius);
			}

			for (Uint32 i = 0; i < m_test.size(); ++i)
				m_test[i].sync(m_ui->state.globe.radius);
			for (Uint32 i = 0; i < m_ui->state.bases.size(); ++i)
				m_ui->state.bases[i]->getLocation().sync(m_ui->state.globe.radius);
		}
	}

	void Globe::setDefaultTarget(Sint16 sx, Sint16 sy)
	{
		Point3d p;
		if (!screenToCartesian(sx, sy, p))
			return;

		p.toSpherical(m_defaultTarget, m_ui->state.globe.radius);
	}

	bool Globe::onMouseLeftClick(Sint16 _x, Sint16 _y)
	{
		if (m_mode == CreateBase)
		{
			vector<GeoPolygon>::iterator i = screenToPolygon(_x, _y);

			GeoPoint p;
			if (screenToCartesian(_x, _y, p.c) && i != m_polygons.end())
			{
				p.c.toSpherical(p.s, m_ui->state.globe.radius);

				m_mode = Normal;
				m_newBaseDialog->create(new NewBaseNameDialog(p));
			}
		}
		else
		{
			GeoObject gp;
			if (screenToCartesian(_x, _y, gp.c))
			{
				// populate spherical coordinates
				gp.c.toSpherical(gp.s, m_ui->state.globe.radius);

				// set target
				gp.target = m_defaultTarget;

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
		if (screenToCartesian(x, y, gp.c))
		{
			gp.c.toSpherical(gp.s, m_ui->state.globe.radius);

			m_ui->state.globe.rotx = gp.s.y - 720;
			m_ui->state.globe.rotz = gp.s.x + 720;
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

	bool Globe::screenToSpherical(Sint16 _x, Sint16 _y, Point2d& p)
	{
		Point3d temp;
		if (!screenToCartesian(_x, _y, temp))
			return false;

		temp.toSpherical(p, m_ui->state.globe.radius);
		return true;
	}
}