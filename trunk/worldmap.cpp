#include "worldmap.h"
#include "util.h"
#include <fstream>
#include <SDL_gfxPrimitives.h>
#include <cmath>

namespace ufo
{
	WorldMap::WorldMap()
		: m_rotx(0), m_rotz(720), m_polarDegFix(120), UIElement(0, 0, 256, 200), m_palette("geodata/palettes.dat", 256), m_font("geodata/smallset.dat", 8, 9)
	{
		m_radius = h / 2 - 10;
		m_center = Point2d(w / 2, h / 2);

		m_bg = loadSCR("geograph/geobord.scr");
		m_palette.apply(m_bg);

		m_font.palette(&m_palette);
		m_font.offset(239);

		m_radiusMin = h / 2 - 10;
		m_radiusMax = h * 4;
		m_radius = m_radiusMin;

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

	void WorldMap::draw(SDL_Surface* surface)
	{
		SDL_BlitSurface(m_bg, NULL, surface, NULL);

		SDL_Rect clip;
		SDL_SetRect(&clip, 0, 0, surface->w - 64, surface->h);
		SDL_SetClipRect(surface, &clip);

		for (Uint32 i = 0; i < m_world.size(); ++i)
		{
			for (Uint32 j = 0; j < m_world[i].size(); ++j)
			{
				Uint32 k = (j + 1) % m_world[i].size();

				Point3d p1(m_world[i][j].c);
				Point3d p2(m_world[i][k].c);

				// perform rotation
				rotate(p1, m_rotx, m_rotz);
				rotate(p2, m_rotx, m_rotz);

				// skip if points are on back side of sphere
				if (p1.y > 0 && p2.y > 0)
					continue;

				Point2d p3;
				project(p1, p3);

				Point2d p4;
				project(p2, p4);

				lineColor(surface, round(p3.x), round(p3.y), round(p4.x), round(p4.y), GetColor(50, 50, 180));
			}
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

			drawShip(surface, round(p2.x), round(p2.y), GetColor(255, 255, 0));
		}

		Point3d p1;
		toCartesian(m_defaultTarget, p1);

		rotate(p1, m_rotx, m_rotz);
		if (p1.y <= 0)
		{
			Point2d p2;
			project(p1, p2);

			drawShip(surface, round(p2.x), round(p2.y), GetColor(255, 0, 0));
		}

		SDL_SetClipRect(surface, NULL);

		m_font.write(surface, 5, 5, "Rotation (x, z): %d, %d", m_rotx, m_rotz);

		GeoPoint gptemp;
		if (screenToCartesian(m_mx, m_my, gptemp.c))
		{
			toSpherical(gptemp.c, gptemp.s);
			m_font.write(surface, 5, 15, "Mouse -> Spherical: %f, %f", gptemp.s.x, gptemp.s.y);
			m_font.write(surface, 5, 25, "Mouse -> Cartesian: %f, %f, %f", gptemp.c.x, gptemp.c.y, gptemp.c.z);
			m_font.write(surface, 5, 45, "toSpherical test: %f, %f", gptemp.c.z, toDeg(acos(gptemp.c.z / m_radius)));
			m_font.write(surface, 5, 55, "Radius: %d", m_radius);
		}

		m_font.write(surface, 5, 35, "Default Target (Spherical): %f, %f", m_defaultTarget.x, m_defaultTarget.y);
	}

	void WorldMap::drawShip(SDL_Surface* surface, Sint16 x, Sint16 y, Uint32 color)
	{
		pixelColor(surface, x + 1, y, color);
		pixelColor(surface, x - 1, y, color);
		pixelColor(surface, x, y + 1, color);
		pixelColor(surface, x, y - 1, color);
	}

	// convert Spherical coordinates to Cartesian coordinates/
	void WorldMap::toCartesian(const Point2d& p1, Point3d& p2)
	{
		double sx = sin(toRad(p1.x));
		double cx = cos(toRad(p1.x));
		double sy = sin(toRad(p1.y));
		double cy = cos(toRad(p1.y));

		p2.x = m_radius * sy * cx;
		p2.y = m_radius * sy * sx;
		p2.z = m_radius * cy;
	}

	// convert Cartesian coordinates to Spherical coordinates
	void WorldMap::toSpherical(const Point3d& p1, Point2d& p2)
	{
		p2.y = toDeg(acos(p1.z / m_radius));
		p2.x = toDeg(atan2(p1.y, p1.x));
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
		p2.x = static_cast<Sint16>(p1.x + m_center.x);
		p2.y = static_cast<Sint16>(-p1.z + m_center.y);
	}

	double WorldMap::distance(Point2d p1, Point2d p2)
	{
		return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
	}

	void WorldMap::center(Sint16 sx, Sint16 sy)
	{
		GeoPoint gp;
		if (!screenToCartesian(sx, sy, gp.c))
			return;

		toSpherical(gp.c, gp.s);

		m_rotx = gp.s.y - 720;
		m_rotz = gp.s.x + 720;
	}

	void WorldMap::onClick(Sint16 sx, Sint16 sy)
	{
		GeoObject gp;
		if (!screenToCartesian(sx, sy, gp.c))
			return;

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

	void WorldMap::zoom(Sint16 delta)
	{
		m_radius += delta;
		if (m_radius < m_radiusMin)
			m_radius = m_radiusMin;
		if (m_radius > m_radiusMax)
			m_radius = m_radiusMax;

		for (Uint32 i = 0; i < m_world.size(); ++i)
		{
			for (Uint32 j = 0; j < m_world[i].size(); ++j)
				toCartesian(m_world[i][j].s, m_world[i][j].c);
		}

		for (Uint32 i = 0; i < m_test.size(); ++i)
			toCartesian(m_test[i].s, m_test[i].c);
	}

	void WorldMap::setDefaultTarget(Sint16 sx, Sint16 sy)
	{
		Point3d p;
		if (!screenToCartesian(sx, sy, p))
			return;

		toSpherical(p, m_defaultTarget);
	}

	bool WorldMap::processEvent(SDL_Event& e)
	{
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (e.button.button == SDL_BUTTON_LEFT)
				onClick(e.button.x, e.button.y);
			if (e.button.button == SDL_BUTTON_RIGHT)
				center(e.button.x, e.button.y);

			return true;
		}
		if (e.type == SDL_MOUSEMOTION)
		{
			m_mx = e.motion.x;
			m_my = e.motion.y;
		}
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_UP)
				rotateVert(-8);
			if (e.key.keysym.sym == SDLK_DOWN)
				rotateVert(8);
			if (e.key.keysym.sym == SDLK_LEFT)
				rotateHorz(-8);
			if (e.key.keysym.sym == SDLK_RIGHT)
				rotateHorz(8);
			if (e.key.keysym.sym == SDLK_PAGEUP)
				zoom(10);
			if (e.key.keysym.sym == SDLK_PAGEDOWN)
				zoom(-10);
			if (e.key.keysym.sym == SDLK_SPACE)
				setDefaultTarget(m_mx, m_my);
		}

		return false;
	}
}