#include "worldmap.h"
#include <fstream>
#include <sdl_gfxprimitives.h>
#include <cmath>

namespace ufo
{
	void TrigTable::load(string filename)
	{
		ifstream file(filename.c_str(), ios::binary);
		if (!file)
			throw runtime_error("error opening " + filename);

		m_table.resize(2881);
		for (size_t i = 0; i < m_table.size(); ++i)
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
		: m_surface(surface), m_radius(surface->h / 2 - 10), m_rotx(0), m_rotz(0)
	{
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
		for (size_t i = 0; i < m_world.size(); ++i)
		{
			for (size_t j = 0; j < m_world[i].size(); ++j)
			{
				size_t k = (j + 1) % m_world[i].size();

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

				lineRGBA(m_surface, p3.x, p3.y, p4.x, p4.y, 50, 50, 180, 255);
			}
		}

		for (size_t i = 0; i < m_test.size(); ++i)
		{
			// move point toward 0,0
			if (SDL_GetTicks() - m_test[i].lastUpdate > 20)
			{
				Point2d temp(m_test[i].s);
				temp.x += static_cast<Sint16>(cos(m_test[i].direction) * 5);
				temp.y += static_cast<Sint16>(sin(m_test[i].direction) * 5);

//				if (distance(temp, m_test[i].target) < m_test[i].lastDistance)
//				{
					m_test[i].s = temp;
					toCartesian(m_test[i].s, m_test[i].c);
//				}

				m_test[i].lastUpdate = SDL_GetTicks();
			}

			Point3d p1(m_test[i].c);
			rotate(p1, m_rotx, m_rotz);
			if (p1.y > 0)
				continue;

			Point2d p2;
			project(p1, p2);

			drawShip(p2.x, p2.y, SDL_MapRGB(m_surface->format, 200, 255, 200));
		}

		Point3d p1;
		toCartesian(m_defaultTarget, p1);

		Point2d p2;
		project(p1, p2);

		drawShip(p2.x, p2.y, SDL_MapRGB(m_surface->format, 255, 150, 150));
	}

	void WorldMap::drawShip(Sint16 x, Sint16 y, Uint32 color)
	{
		pixelColor(m_surface, x, y, color);
		pixelColor(m_surface, x + 1, y, color);
		pixelColor(m_surface, x - 1, y, color);
		pixelColor(m_surface, x, y + 1, color);
		pixelColor(m_surface, x, y - 1, color);
	}

	// convert Spherical coordinates to Cartesian coordinates
	void WorldMap::toCartesian(const Point2d& p1, Point3d& p2)
	{
		double sx = m_sin(p1.x) / 1024.0;
		double cx = m_cos(p1.x) / 1024.0;
		double sy = m_sin(p1.y) / 1024.0;
		double cy = m_cos(p1.y) / 1024.0;

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
		p.x = x - m_surface->w / 2;
		p.z = -(y - m_surface->h / 2);

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
		p.y = m_cos(z) / 1024.0 * orig.y - m_sin(z) / 1024.0 * orig.x;
		p.x = m_sin(z) / 1024.0 * orig.y + m_cos(z) / 1024.0 * orig.x;

		// rotate x-axis
		orig = p;
		p.z = m_cos(x) / 1024.0 * orig.z - m_sin(x) / 1024.0 * orig.y;
		p.y = m_sin(x) / 1024.0 * orig.z + m_cos(x) / 1024.0 * orig.y;
	}

	void WorldMap::project(const Point3d& p1, Point2d& p2)
	{
		p2.x = static_cast<Sint16>(p1.x + m_surface->w / 2);
		p2.y = static_cast<Sint16>(-p1.z + m_surface->h / 2);
	}

	Sint16 WorldMap::distance(Point2d p1, Point2d p2)
	{
		return abs((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
	}

	void WorldMap::onClick(Sint16 sx, Sint16 sy)
	{
		GeoObject gp;
		screenToCartesian(sx, sy, gp.c);

		// populate spherical coordinates
		toSpherical(gp.c, gp.s);

		// set direction to target
		gp.target = m_defaultTarget;

		Point2d orig(gp.s);
		Point2d adj(gp.s);
		adj.x += 2880;

		gp.s = (distance(orig, gp.target) < distance(adj, gp.target) ? orig : adj);
		gp.direction = atan2(static_cast<double>(gp.target.y - gp.s.y), static_cast<double>(gp.target.x - gp.s.x));
		gp.lastDistance = distance(gp.s, gp.target);

		gp.lastUpdate = SDL_GetTicks();

		toCartesian(gp.s, gp.c);
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

	void WorldMap::setDefaultTarget(Sint16 sx, Sint16 sy)
	{
		Point3d p;
		screenToCartesian(sx, sy, p);

		toSpherical(p, m_defaultTarget);
	}
}