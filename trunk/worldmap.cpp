#include "worldmap.h"
#include "util.h"
#include <fstream>
#include <cmath>
#include <sdl_gfxprimitives.h>

namespace ufo
{
	WorldMap::WorldMap(Surface& surface)
		: m_rotx(0), m_rotz(720), m_polarDegFix(120), UIElement(0, 0, 256, 200), m_palette("geodata/palettes.dat", 256, 0), m_mx(0), m_my(0), m_zoom(0)
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

		for (size_t i = 26; i < 39; ++i)
		{
			m_textures.push_back(new Surface(SDL_CreateRGBSurface(SDL_HWSURFACE, 32, 32, 8, 0, 0, 0, 0)));
			Rect r(texture.getFrameRect(i));
			m_palette.apply(*m_textures.back());
			texture.blit(*m_textures.back(), 0, &r);
		}

		// apply palette to output surface
		m_palette.apply(surface);

		// set the font color (palette index) offset
		m_font.colorOffset(239);

		Rect rInv(257, 0, 63, 11);
		m_bg.invert(248, &rInv);

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
			m_world.push_back(polygon);
		}

		m_defaultTarget.x = 0;
		m_defaultTarget.y = 0;
	}

	void WorldMap::draw(Surface& surface)
	{
		// draw background
		m_bg.blit(surface);

		// set clipping rectangle
		surface.setClipRect(Rect(0, 0, w, h));

		// draw ocean
		filledCircleColor(surface.get(), m_center.x, m_center.y, m_radius - 1, GetColor(0, 0, 255));

		vector<Sint16> vx(4), vy(4);
		for (size_t i = 0; i < m_world.size(); ++i)
		{
			size_t hidden = 0;
			Sint16* minx = &vx[0];
			Sint16* miny = &vy[0];
			Sint16* maxx = &vx[0];
			Sint16* maxy = &vy[0];
			for (size_t j = 0; j < m_world[i].size(); ++j)
			{
				Point3d p1(m_world[i][j].c);

				// perform rotation
				rotate(p1, m_rotx, m_rotz);

				// check if point is on back of sphere
				if (p1.y > 0)
					++hidden;

				Point2d p2;
				project(p1, p2);

				vx[j] = p2.x;
				vy[j] = p2.y;

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
			if (hidden == m_world[i].size())
				continue;

			// enlarge polygons to remove gaps
			(*minx) -= 1;
			(*miny) -= 1;
			(*maxx) += 1;
			(*maxy) += 1;

			texturedPolygon(surface.get(), &vx[0], &vy[0], m_world[i].size(), m_textures[m_world[i].texture]->get(), 0, 0);
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
					m_test[i].s.x += static_cast<Sint16>(cos(direction) * 2);
					m_test[i].s.y += static_cast<Sint16>(sin(direction) * 2);

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
		if (screenToCartesian(m_mx, m_my, gptemp.c))
		{
			toSpherical(gptemp.c, gptemp.s);
			m_font.printf(surface, 5, 15, "Mouse -> Spherical: %f, %f", gptemp.s.x, gptemp.s.y);
			m_font.printf(surface, 5, 25, "Mouse -> Cartesian: %f, %f, %f", gptemp.c.x, gptemp.c.y, gptemp.c.z);
			m_font.printf(surface, 5, 45, "Radius: %d", m_radius);
		}

		m_font.printf(surface, 5, 35, "Default Target (Spherical): %f, %f", m_defaultTarget.x, m_defaultTarget.y);

		m_font.printf(surface, 5, 55, "Pixel: %d", surface.getPixel8(m_mx, m_my));
	}

	void WorldMap::drawShip(Surface& surface, Sint16 x, Sint16 y, Uint8 color)
	{
		surface.pixelColor8(x + 1, y, color);
		surface.pixelColor8(x - 1, y, color);
		surface.pixelColor8(x, y + 1, color);
		surface.pixelColor8(x, y - 1, color);
	}

	// convert Spherical coordinates to Cartesian coordinates
	void WorldMap::toCartesian(const Point2d& p1, Point3d& p2)
	{
		double rx = toRad(p1.x);
		double ry = toRad(p1.y);
		double sy = sin(ry);

		p2.x = m_radius * sy * cos(rx);
		p2.y = m_radius * sy * sin(rx);
		p2.z = m_radius * cos(ry);
	}

	// convert Cartesian coordinates to Spherical coordinates
	void WorldMap::toSpherical(const Point3d& p1, Point2d& p2)
	{
		p2.y = round<Sint16>(toDeg(acos(p1.z / m_radius)));
		p2.x = round<Sint16>(toDeg(atan2(p1.y, p1.x)));
	}

	// convert Screen coordinates to Cartesian (x,y,z)
	bool WorldMap::screenToCartesian(Sint16 x, Sint16 y, Point3d& p)
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

	void WorldMap::rotate(Point3d& p, Sint16 x, Sint16 z)
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

	void WorldMap::project(const Point3d& p1, Point2d& p2)
	{
		p2.x = round<Sint16>(p1.x + m_center.x);
		p2.y = round<Sint16>(-p1.z + m_center.y);
	}

	double WorldMap::distance(Point2d p1, Point2d p2)
	{
		return sqrt(static_cast<double>((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y)));
	}

	void WorldMap::rotateHorz(Sint16 delta)
	{
		m_rotz += delta;
	}

	void WorldMap::rotateVert(Sint16 delta)
	{
		m_rotx += delta;
		if (m_rotx < -720)
			m_rotx = -720;
		else if (m_rotx > 720)
			m_rotx = 720;
	}

	void WorldMap::zoom(Sint8 delta)
	{
		if (m_zoom + delta < m_zoomLevels.size())
		{
			m_zoom += delta;
			m_radius = m_zoomLevels[m_zoom];

			for (Uint32 i = 0; i < m_world.size(); ++i)
			{
				for (Uint32 j = 0; j < m_world[i].size(); ++j)
					toCartesian(m_world[i][j].s, m_world[i][j].c);
			}

			for (Uint32 i = 0; i < m_test.size(); ++i)
				toCartesian(m_test[i].s, m_test[i].c);
		}
	}

	void WorldMap::setDefaultTarget(Sint16 sx, Sint16 sy)
	{
		Point3d p;
		if (!screenToCartesian(sx, sy, p))
			return;

		toSpherical(p, m_defaultTarget);
	}

	bool WorldMap::onMouseLeftClick(Sint16 x, Sint16 y)
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

	bool WorldMap::onMouseRightClick(Sint16 x, Sint16 y)
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

	bool WorldMap::onMouseHover(Sint16 x, Sint16 y)
	{
		m_mx = x;
		m_my = y;

		return true;
	}

	bool WorldMap::onKeyDown(SDL_keysym keysym)
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
			setDefaultTarget(m_mx, m_my);

		return false;
	}
}