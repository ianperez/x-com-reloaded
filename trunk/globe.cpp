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

	public:

		NewBaseNameEdit(Sint16 _x, Sint16 _y)
			: UIEdit(m_font, _x, _y, 140, 13, true) { }

		void onEnter()
		{
			m_ui->state.bases.back()->setName(m_buffer);
			m_ui->destroy(m_parent);
		}
	};

	class NewBaseNameDialog : public UIDialog
	{
	public:

		NewBaseNameDialog()
			: UIDialog(32, 60, 192, 80, Palette::blockSize * 8 + 6, UIDialog::Both) { }

		void onOpen()
		{
			m_bg.loadSCR("geograph/back01.scr");

			Palette backPalette("geodata/backpals.dat", 0, 16);
			backPalette.apply(m_bg);

			m_parent->create(new NewBaseNameEdit(x + 22, y + 50));
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

	Globe::Globe(StartMode mode)
		: m_rotx(0), m_rotz(720), m_polarDegFix(120), UIElement(0, 0, 256, 200), m_palette("geodata/palettes.dat"), m_mouse(0, 0), m_zoom(0), m_debug(false), m_mode(mode), m_currentPolygon(0)
	{
	}

	void Globe::onCreate()
	{
		// set up zoom levels
		m_zoomLevels.push_back(91);
		m_zoomLevels.push_back(125);
		m_zoomLevels.push_back(230);
		m_zoomLevels.push_back(360);
		m_zoomLevels.push_back(480);
		m_zoomLevels.push_back(630);

		m_radius = m_zoomLevels[0];

		// set center of globe
		m_center = Point2d(w / 2, h / 2);

		// load globe textures
		Surface texture;
		texture.loadSCR("geograph/texture.dat", 32);
		m_palette.apply(texture);

		// set texture frames (each texture is 32x32)
		texture.setFrames(32, 32, 39);

		for (size_t i = 0; i < 39; ++i)
		{
			m_textures.push_back(shared_ptr<Surface>(new Surface(SDL_CreateRGBSurface(SDL_HWSURFACE, 32, 32, 8, 0, 0, 0, 0))));
			Rect r(texture.getFrameRect(i));
			m_palette.apply(*m_textures.back());
			texture.blit(*m_textures.back(), 0, &r);
		}

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

				gp.s.toCartesian(gp.c, m_radius);

				polygon.push_back(gp);
			}

			if (file.eof())
				break;

			file.read((char*)&polygon.texture, 4);
			m_polygons.push_back(polygon);
		}

		m_defaultTarget.x = 0;
		m_defaultTarget.y = 0;

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

					m_test[i].s.toCartesian(m_test[i].c, m_radius);
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
		filledCircleColor(surface.get(), m_center.x, m_center.y, m_radius - 1, m_palette.getRGBA(Palette::blockSize * 12));

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
				rotate(p1, m_rotx, m_rotz);

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
				texturedPolygon(surface.get(), &vx[0], &vy[0], m_polygons[i].size(), m_textures[m_polygons[i].texture + 13 * ((5 - m_zoom) / 2)]->get(), 0, 0);
		}

		m_currentPolygon %= m_polygons.size();
		for (size_t i = 0; i < m_polygons[m_currentPolygon].size(); ++i)
		{
			Point3d p1(m_polygons[m_currentPolygon][i].c);

			// perform rotation
			rotate(p1, m_rotx, m_rotz);

			// check if point is on back of sphere
			if (p1.y > 0)
				break;

			Point2d p2;
			project(p1, p2);

			m_font.print(surface, p2.x, p2.y, format("%d,%d", m_polygons[m_currentPolygon][i].s.x, m_polygons[m_currentPolygon][i].s.y));
		}

		// set clipping rectangle
		surface.setClipRect(*this);

		// draw bases
		for (Uint32 i = 0; i < m_ui->state.bases.size(); ++i)
		{
			Point3d p1(m_ui->state.bases[i]->getLocation().c);
			rotate(p1, m_rotx, m_rotz);
			if (p1.y > 0)
				continue;

			Point2d p2;
			project(p1, p2);

			m_ui->state.bases[i]->draw(surface, p2);
		}

		for (Uint32 i = 0; i < m_test.size(); ++i)
		{
			Point3d p1(m_test[i].c);
			rotate(p1, m_rotx, m_rotz);
			if (p1.y > 0)
				continue;

			Point2d p2;
			project(p1, p2);

			drawShip(surface, p2.x, p2.y, 11);
		}

		Point3d p1;
		m_defaultTarget.toCartesian(p1, m_radius);

		rotate(p1, m_rotx, m_rotz);
		if (p1.y <= 0)
		{
			Point2d p2;
			project(p1, p2);

			drawShip(surface, p2.x, p2.y, 13);
		}

		if (m_debug)
		{
			m_font.print(surface, 5, 5, format("Rotation (x, z): %d, %d", m_rotx, m_rotz));

			GeoPoint gptemp;
			if (screenToCartesian(m_mouse.x, m_mouse.y, gptemp.c))
			{
				gptemp.c.toSpherical(gptemp.s, m_radius);
				m_font.print(surface, 5, 15, format("Mouse -> Spherical: %d, %d", gptemp.s.x, gptemp.s.y));
				m_font.print(surface, 5, 25, format("Mouse -> Cartesian: %f, %f, %f", gptemp.c.x, gptemp.c.y, gptemp.c.z));
			}

			m_font.print(surface, 5, 45, format("Radius: %d", m_radius));
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

		double ys = m_radius * m_radius - p.x * p.x - p.z * p.z;
		if (ys < 0)
			return false;

		p.y = -sqrt(ys);

		// reverse rotation
		rotate(p, -m_rotx, 0);
		rotate(p, 0, -m_rotz);

		return true;
	}

	bool Globe::screenToPolygon(Sint16 _x, Sint16 _y, GeoPolygon* gp)
	{
		GeoPoint p;
		if (screenToCartesian(_x, _y, p.c))
		{
			p.c.toSpherical(p.s, m_radius);

			for (size_t i = 0; i < m_polygons.size(); ++i)
			{
				bool lastResult;
				bool found = true;
				for (size_t j = 0; j < m_polygons[i].size(); ++j)
				{
					size_t k = (j + 1) % m_polygons[i].size();

					Point2d p1(m_polygons[i][j].s);
					Point2d p2(m_polygons[i][k].s);

					if (j > 0)
						p1.adjust(m_polygons[i][0].s);
					p2.adjust(m_polygons[i][0].s);

					Sint32 v = (p.s.y - p1.y) * (p2.x - p1.x) - (p.s.x - p1.x) * (p2.y - p1.y);
					if (j == 0)
						lastResult = (v < 0);
					else if (lastResult != (v < 0))
					{
						found = false;
						break;
					}
				}

				if (found)
				{
					if (gp)
						*gp = m_polygons[i];

					m_polygons[i].texture = 100;
					return true;
				}
			}
		}

		return false;
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
		m_rotz = (m_rotz + delta) % 2880;
	}

	void Globe::rotateVertical(Sint16 delta)
	{
		m_rotx = (m_rotx + delta) % 2880;
	}

	void Globe::zoom(Sint8 delta)
	{
		if (m_zoom + delta < m_zoomLevels.size())
		{
			m_zoom += delta;
			m_radius = m_zoomLevels[m_zoom];

			for (Uint32 i = 0; i < m_polygons.size(); ++i)
			{
				for (Uint32 j = 0; j < m_polygons[i].size(); ++j)
					m_polygons[i][j].s.toCartesian(m_polygons[i][j].c, m_radius);
			}

			for (Uint32 i = 0; i < m_test.size(); ++i)
				m_test[i].s.toCartesian(m_test[i].c, m_radius);
		}
	}

	void Globe::setDefaultTarget(Sint16 sx, Sint16 sy)
	{
		Point3d p;
		if (!screenToCartesian(sx, sy, p))
			return;

		p.toSpherical(m_defaultTarget, m_radius);
	}

	bool Globe::onMouseLeftClick(Sint16 _x, Sint16 _y)
	{
		if (m_mode == CreateBase)
		{
			GeoPoint p;
			if (screenToCartesian(_x, _y, p.c))
			{
				p.c.toSpherical(p.s, m_radius);

				m_ui->state.bases.push_back(shared_ptr<Base>(new Base));
				m_ui->state.bases.back()->setLocation(p);

				m_mode = Normal;
				m_newBaseDialog->create(new NewBaseNameDialog());
			}
		}
		else
		{
			GeoObject gp;
			if (screenToCartesian(_x, _y, gp.c))
			{
				// populate spherical coordinates
				gp.c.toSpherical(gp.s, m_radius);

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
			gp.c.toSpherical(gp.s, m_radius);

			m_rotx = gp.s.y - 720;
			m_rotz = gp.s.x + 720;
		}

		return true;
	}

	bool Globe::onMouseMove(Sint16 x, Sint16 y)
	{
		m_mouse.x = x;
		m_mouse.y = y;

		screenToPolygon(x, y);

		return true;
	}

	bool Globe::onKeyDown(SDL_keysym keysym)
	{
		if (keysym.sym == SDLK_F12)
			m_debug = !m_debug;
		if (keysym.sym == SDLK_SPACE)
			setDefaultTarget(m_mouse.x, m_mouse.y);
		if (keysym.sym == SDLK_LEFT)
			m_currentPolygon--;
		if (keysym.sym == SDLK_RIGHT)
			m_currentPolygon++;

		return false;
	}

	bool Globe::screenToSpherical(Sint16 _x, Sint16 _y, Point2d& p)
	{
		Point3d temp;
		if (!screenToCartesian(_x, _y, temp))
			return false;

		temp.toSpherical(p, m_radius);
		return true;
	}
}