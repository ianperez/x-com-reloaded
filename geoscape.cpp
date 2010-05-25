#include "GeoScape.h"
#include "util.h"
#include "uimanager.h"
#include <fstream>
#include <cmath>
#include <sdl_gfxprimitives.h>

namespace ufo
{
	GeoScapeButton::GeoScapeButton(GeoScape& gs, Sint16 _x, Sint16 _y, Uint16 id)
		: m_gs(gs), UIPushButton(_x, _y, 63, 11)
	{
		m_id = id;
	}

	void GeoScapeButton::onPress()
	{
		if (m_id == Intercept)
		{
			m_ui->create(new InterceptDialog(m_ui->surface));
		}
	}

	void GeoScapeButton::draw(Surface& surface)
	{
		if (m_pressed)
			surface.invert(248, this);
	}

	GeoScapeTimeButton::GeoScapeTimeButton(GeoScape& gs, Sint16 _x, Sint16 _y, Uint16 id)
		: m_gs(gs), UIRadioButton(_x, _y, 31, 13, 0)
	{
		m_id = id;
	}

	void GeoScapeTimeButton::onPress()
	{
	}

	void GeoScapeTimeButton::draw(Surface& surface)
	{
		if (m_pressed)
			surface.invert(248, this);
	}

	InterceptDialog::InterceptDialog(Surface& surface)
		: UIDialog(surface, 320, 140, Palette::blockSize * 15, UIDialog::Horizontal)
	{
		m_bg.loadSCR("geograph/back12.scr");

		Palette p("geodata/backpals.dat", 4, 16);
		p.apply(m_bg);
		p.apply(surface);
	}

	void InterceptDialog::onOpen()
	{
	}

	GeoScape::GeoScape()
		: m_rotx(0), m_rotz(720), m_polarDegFix(120), UIElement(0, 0, 256, 200), m_palette("geodata/palettes.dat"), m_mouse(0, 0), m_zoom(0)
	{
	}

	void GeoScape::onCreate()
	{
		// set up zoom levels
		m_zoomLevels.push_back(91);
		m_zoomLevels.push_back(121);
		m_zoomLevels.push_back(210);
		m_zoomLevels.push_back(320);
		m_zoomLevels.push_back(430);
		m_zoomLevels.push_back(540);

		m_radius = m_zoomLevels[0];

		// set center of globe
		m_center = Point2d(w / 2, h / 2);

		// load geoscape background image
		m_bg.loadSCR("geograph/geobord.scr");
		m_palette.apply(m_bg);

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

		// apply palette to output surface
		m_palette.apply(m_ui->surface);

		// set the font color (palette index) offset
		m_font.colorOffset(Palette::blockSize * 14 + 15);

		// create buttons
		m_ui->create(new GeoScapeButton(*this, 257, 0, GeoScapeButton::Intercept));
		m_ui->create(new GeoScapeButton(*this, 257, 12, GeoScapeButton::Bases));
		m_ui->create(new GeoScapeButton(*this, 257, 24, GeoScapeButton::Graphs));
		m_ui->create(new GeoScapeButton(*this, 257, 36, GeoScapeButton::Ufopaedia));
		m_ui->create(new GeoScapeButton(*this, 257, 48, GeoScapeButton::Options));
		m_ui->create(new GeoScapeButton(*this, 257, 60, GeoScapeButton::Funding));

		m_ui->create(new GeoScapeTimeButton(*this, 257, 112, GeoScapeTimeButton::Time5Sec));
		m_ui->create(new GeoScapeTimeButton(*this, 289, 112, GeoScapeTimeButton::Time1Min));
		m_ui->create(new GeoScapeTimeButton(*this, 257, 126, GeoScapeTimeButton::Time5Min));
		m_ui->create(new GeoScapeTimeButton(*this, 289, 126, GeoScapeTimeButton::Time30Min));
		m_ui->create(new GeoScapeTimeButton(*this, 257, 140, GeoScapeTimeButton::Time1Hour));
		m_ui->create(new GeoScapeTimeButton(*this, 289, 140, GeoScapeTimeButton::Time1Day));

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
				Sint16 v;
				file.read((char*)&v, 2);
				if (file.eof())
					break;

				gp.s.x = v;

				file.read((char*)&v, 2);

				if (gp.s.x == -1)
					break;

				gp.s.y = v + 720;

				toCartesian(gp.s, gp.c);

				polygon.push_back(gp);
			}

			if (file.eof())
				break;

			file.read((char*)&polygon.texture, 4);
			m_polygons.push_back(polygon);
		}

		m_defaultTarget.x = 0;
		m_defaultTarget.y = 0;
	}

	void GeoScape::draw(Surface& surface)
	{
		// draw background
		m_bg.blit(surface);

		// set clipping rectangle
		surface.setClipRect(Rect(0, 0, w, h));

		// draw ocean
		filledCircleColor(surface.get(), m_center.x, m_center.y, m_radius - 1, GetColor(0, 0, 255));

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

			texturedPolygon(surface.get(), &vx[0], &vy[0], m_polygons[i].size(), m_textures[m_polygons[i].texture + 13 * ((5 - m_zoom) / 2)]->get(), 0, 0);
		}

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

					toCartesian(m_test[i].s, m_test[i].c);
				}

				m_test[i].lastUpdate = SDL_GetTicks();
			}

			Point3d p1(m_test[i].c);
			rotate(p1, m_rotx, m_rotz);
			if (p1.y > 0)
				continue;

			Point2d p2;
			project(p1, p2);

			drawShip(surface, p2.x, p2.y, 11);
		}

		Point3d p1;
		toCartesian(m_defaultTarget, p1);

		rotate(p1, m_rotx, m_rotz);
		if (p1.y <= 0)
		{
			Point2d p2;
			project(p1, p2);

			drawShip(surface, p2.x, p2.y, 13);
		}

		surface.clearClipRect();

		m_font.printf(surface, 5, 5, "Rotation (x, z): %d, %d", m_rotx, m_rotz);

		GeoPoint gptemp;
		if (screenToCartesian(m_mouse.x, m_mouse.y, gptemp.c))
		{
			toSpherical(gptemp.c, gptemp.s);
			m_font.printf(surface, 5, 15, "Mouse -> Spherical: %d, %d", gptemp.s.x, gptemp.s.y);
			m_font.printf(surface, 5, 25, "Mouse -> Cartesian: %f, %f, %f", gptemp.c.x, gptemp.c.y, gptemp.c.z);
		}

		m_font.printf(surface, 5, 45, "Radius: %d", m_radius);
		m_font.printf(surface, 5, 35, "Default Target (Spherical): %d, %d", m_defaultTarget.x, m_defaultTarget.y);

		m_font.printf(surface, 5, 55, "Pixel: %d", surface.getPixel8(m_mouse.x, m_mouse.y));
	}

	void GeoScape::drawShip(Surface& surface, Sint16 x, Sint16 y, Uint8 color)
	{
		surface.pixelColor8(x + 1, y, color);
		surface.pixelColor8(x - 1, y, color);
		surface.pixelColor8(x, y + 1, color);
		surface.pixelColor8(x, y - 1, color);
	}

	// convert Spherical coordinates to Cartesian coordinates
	void GeoScape::toCartesian(const Point2d& p1, Point3d& p2)
	{
		double rx = toRad(p1.x);
		double ry = toRad(p1.y);
		double sy = sin(ry);

		p2.x = m_radius * sy * cos(rx);
		p2.y = m_radius * sy * sin(rx);
		p2.z = m_radius * cos(ry);
	}

	// convert Cartesian coordinates to Spherical coordinates
	void GeoScape::toSpherical(const Point3d& p1, Point2d& p2)
	{
		p2.y = round<Sint16>(toDeg(acos(p1.z / m_radius)));
		p2.x = round<Sint16>(toDeg(atan2(p1.y, p1.x)));
	}

	// convert Screen coordinates to Cartesian (x,y,z)
	bool GeoScape::screenToCartesian(Sint16 x, Sint16 y, Point3d& p)
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

	void GeoScape::rotate(Point3d& p, Sint16 x, Sint16 z)
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

	void GeoScape::project(const Point3d& p1, Point2d& p2)
	{
		p2.x = round<Sint16>(p1.x + m_center.x);
		p2.y = round<Sint16>(-p1.z + m_center.y);
	}

	double GeoScape::distance(Point2d p1, Point2d p2)
	{
		return sqrt(static_cast<double>((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y)));
	}

	void GeoScape::rotateHorz(Sint16 delta)
	{
		m_rotz += delta;
	}

	void GeoScape::rotateVert(Sint16 delta)
	{
		m_rotx += delta;
	}

	void GeoScape::zoom(Sint8 delta)
	{
		if (m_zoom + delta < m_zoomLevels.size())
		{
			m_zoom += delta;
			m_radius = m_zoomLevels[m_zoom];

			for (Uint32 i = 0; i < m_polygons.size(); ++i)
			{
				for (Uint32 j = 0; j < m_polygons[i].size(); ++j)
					toCartesian(m_polygons[i][j].s, m_polygons[i][j].c);
			}

			for (Uint32 i = 0; i < m_test.size(); ++i)
				toCartesian(m_test[i].s, m_test[i].c);
		}
	}

	void GeoScape::setDefaultTarget(Sint16 sx, Sint16 sy)
	{
		Point3d p;
		if (!screenToCartesian(sx, sy, p))
			return;

		toSpherical(p, m_defaultTarget);
	}

	bool GeoScape::onMouseLeftClick(Sint16 x, Sint16 y)
	{
		GeoObject gp;
		if (screenToCartesian(x, y, gp.c))
		{
			// populate spherical coordinates
			toSpherical(gp.c, gp.s);

			// set target
			gp.target = m_defaultTarget;

			// adjust target if needed
			if (distance(gp.s, gp.target) > distance(gp.s, Point2d(gp.target.x - 2880, gp.target.y)))
				gp.target.x -= 2880;
			if (distance(gp.s, gp.target) > distance(gp.s, Point2d(gp.target.x + 2880, gp.target.y)))
				gp.target.x += 2880;

			gp.lastUpdate = SDL_GetTicks();
			m_test.push_back(gp);
		}

		return true;
	}

	bool GeoScape::onMouseRightClick(Sint16 x, Sint16 y)
	{
		GeoPoint gp;
		if (screenToCartesian(x, y, gp.c))
		{
			toSpherical(gp.c, gp.s);

			m_rotx = gp.s.y - 720;
			m_rotz = gp.s.x + 720;
		}

		return true;
	}

	bool GeoScape::onMouseHover(Sint16 x, Sint16 y)
	{
		m_mouse.x = x;
		m_mouse.y = y;

		return true;
	}

	bool GeoScape::onKeyDown(SDL_keysym keysym)
	{
		if (keysym.sym == SDLK_UP)
			rotateVert(-8);
		if (keysym.sym == SDLK_DOWN)
			rotateVert(8);
		if (keysym.sym == SDLK_LEFT)
			rotateHorz(-8);
		if (keysym.sym == SDLK_RIGHT)
			rotateHorz(8);
		if (keysym.sym == SDLK_PAGEUP)
			zoom(1);
		if (keysym.sym == SDLK_PAGEDOWN)
			zoom(-1);
		if (keysym.sym == SDLK_SPACE)
			setDefaultTarget(m_mouse.x, m_mouse.y);

		return false;
	}
}