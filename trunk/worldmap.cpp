#include "worldmap.h"
#include "util.h"
#include <fstream>
#include <SDL_gfxPrimitives.h>
#include <cmath>

namespace ufo
{
	void TrigTable::load(string filename)
	{
		ifstream file(filename.c_str(), ios::binary);
		if (!file)
			throw runtime_error("error opening " + filename);

		m_table.resize(2881);
		for (Uint32 i = 0; i < m_table.size(); ++i)
			file.read((char*)&m_table[i], 2);
	}

	Sint16 TrigTable::operator () (Sint16 i)
	{
		if (i < 0)
			i = m_table.size() - (abs(i) % m_table.size());
		return m_table[i % m_table.size()];
	}

	Sin::Sin()
	{
		load("geodata/sine.dat");
	}

	Cos::Cos()
	{
		load("geodata/cos.dat");
	}

	WorldMap::WorldMap(SDL_Surface* surface)
		: m_surface(surface), m_radius(surface->h / 2 - 10), m_rotx(0), m_rotz(0), m_polarDegFix(160), UIElement(0, 0, surface->w, surface->h), m_palette("geodata/palettes.dat", 256), m_center((surface->w - 64) / 2, surface->h / 2)
	{
		m_bg = loadSCR("geograph/geobord.scr", m_palette);

		m_radiusMin = m_surface->h / 2 - 10;
		m_radiusMax = m_surface->h * 4;
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
				file.read((char*)&gp.s.x, 2);
				if (file.eof())
					break;

				file.read((char*)&gp.s.y, 2);

				if (gp.s.x == -1)
					break;

				gp.s.y += 720;

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

	void WorldMap::draw()
	{
		SDL_BlitSurface(m_bg, NULL, m_surface, NULL);

		SDL_Rect clip;
		SDL_SetRect(&clip, 0, 0, m_surface->w - 64, m_surface->h);
		SDL_SetClipRect(m_surface, &clip);

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

				lineColor(m_surface, p3.x, p3.y, p4.x, p4.y, GetColor(50, 50, 180));
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

			drawShip(p2.x, p2.y, GetColor(255, 255, 0));
		}

		Point3d p1;
		toCartesian(m_defaultTarget, p1);

		rotate(p1, m_rotx, m_rotz);
		if (p1.y <= 0)
		{
			Point2d p2;
			project(p1, p2);

			drawShip(p2.x, p2.y, GetColor(255, 0, 0));
		}

		SDL_SetClipRect(m_surface, NULL);
	}

	void WorldMap::drawShip(Sint16 x, Sint16 y, Uint32 color)
	{
		pixelColor(m_surface, x + 1, y, color);
		pixelColor(m_surface, x - 1, y, color);
		pixelColor(m_surface, x, y + 1, color);
		pixelColor(m_surface, x, y - 1, color);
	}

	// convert Spherical coordinates to Cartesian coordinates
	void WorldMap::toCartesian(const Point2d& p1, Point3d& p2)
	{
		double sx = m_sin(p1.x) / 1023.0;
		double cx = m_cos(p1.x) / 1023.0;
		double sy = m_sin(p1.y) / 1023.0;
		double cy = m_cos(p1.y) / 1023.0;

		p2.x = m_radius * sy * cx;
		p2.y = m_radius * sy * sx;
		p2.z = m_radius * cy;
	}

	// convert Cartesian coordinates to Spherical coordinates
	void WorldMap::toSpherical(const Point3d& p1, Point2d& p2)
	{
		p2.y = static_cast<Sint16>(acos(p1.z / m_radius) * 180 / Pi * 8);
		p2.x = static_cast<Sint16>(atan2(p1.y, p1.x) * 180 / Pi * 8);
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
		p.y = m_cos(z) / 1023.0 * orig.y - m_sin(z) / 1023.0 * orig.x;
		p.x = m_sin(z) / 1023.0 * orig.y + m_cos(z) / 1023.0 * orig.x;

		// rotate x-axis
		orig = p;
		p.z = m_cos(x) / 1023.0 * orig.z - m_sin(x) / 1023.0 * orig.y;
		p.y = m_sin(x) / 1023.0 * orig.z + m_cos(x) / 1023.0 * orig.y;
	}

	void WorldMap::project(const Point3d& p1, Point2d& p2)
	{
		p2.x = static_cast<Sint16>(p1.x + m_center.x);
		p2.y = static_cast<Sint16>(-p1.z + m_center.y);
	}

	Sint16 WorldMap::distance(Point2d p1, Point2d p2)
	{
		return static_cast<Sint16>(sqrt(static_cast<double>((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y))));
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
		Sint16 radius_min = m_surface->h / 2 - 10;
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
		return false;
	}
}