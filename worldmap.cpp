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
		: m_surface(surface), radius(m_surface->h / 2 - 10), rx(0), ry(0)
	{
		const string filename("geodata/world.dat");
		ifstream file(filename.c_str(), ios::binary);
		if (!file)
			throw runtime_error("error opening " + filename);

		while (1)
		{
			Point3d point;
			Polygon3d polygon;
			for (char i = 0; i < 4; ++i)
			{
				Sint16 x, y;
				file.read((char*)&x, 2);
				if (file.eof())
					break;

				file.read((char*)&y, 2);

				if (x == -1)
					break;

				Point2d temp;
				temp.x = x;
				temp.y = y;

				toSpherical(temp, point);
				rotate(point, 0, 720);

				polygon.push_back(point);
			}

			if (file.eof())
				break;

			file.read((char*)&polygon.texture, 4);
			m_sphere.push_back(polygon);
		}
	}

	void WorldMap::draw()
	{
		for (size_t i = 0; i < m_sphere.size(); ++i)
		{
			for (size_t j = 0; j < m_sphere[i].size(); ++j)
			{
				size_t k = (j + 1) % m_sphere[i].size();

				Point3d p1(m_sphere[i][j]);
				Point3d p2(m_sphere[i][k]);

				// perform rotation
				rotate(p1, rx, ry);
				rotate(p2, rx, ry);

				// skip if points are on back side of sphere
				if (p1.z > 0 && p2.z > 0)
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
			Point3d p1(m_test[i]);
			rotate(p1, rx, ry);
			if (p1.z > 0)
				continue;

			Point2d p2;
			project(p1, p2);

			SDL_Rect r;
			r.x = p2.x - 1;
			r.y = p2.y - 1;
			r.w = r.h = 3;
			SDL_FillRect(m_surface, &r, SDL_MapRGB(m_surface->format, 150, 255, 150));
		}
	}

	void WorldMap::toSpherical(const Point2d& p1, Point3d& p2)
	{
		double sx = m_sin(p1.x) / 1024.0;
		double cx = m_cos(p1.x) / 1024.0;
		double sy = m_sin(p1.y + 720) / 1024.0;
		double cy = m_cos(p1.y + 720) / 1024.0;

		p2.x = radius * sy * cx;
		p2.y = radius * sy * sx;
		p2.z = radius * cy;
	}

	void WorldMap::rotate(Point3d& p, Sint16 x, Sint16 y)
	{
		Point3d orig;

		// rotate y-axis
		orig = p;
		p.z = m_cos(x) / 1024.0 * orig.z - m_sin(x) / 1024.0 * orig.x;
		p.x = m_sin(x) / 1024.0 * orig.z + m_cos(x) / 1024.0 * orig.x;

		// rotate x-axis
		orig = p;
		p.y = m_cos(y) / 1024.0 * orig.y - m_sin(y) / 1024.0 * orig.z;
		p.z = m_sin(y) / 1024.0 * orig.y + m_cos(y) / 1024.0 * orig.z;
	}

	void WorldMap::project(const Point3d& p1, Point2d& p2)
	{
		p2.x = static_cast<Sint16>(p1.x + m_surface->w / 2);
		p2.y = static_cast<Sint16>(p1.y + m_surface->h / 2);
	}

	void WorldMap::onClick(Sint16 sx, Sint16 sy)
	{
		// unproject point
		sx -= m_surface->w / 2;
		sy -= m_surface->h / 2;

		Point3d p;
		p.x = sx;
		p.y = sy;
		double zs = radius * radius - p.x * p.x - p.y * p.y;
		if (zs < 0)
			return;

		p.z = -sqrt(zs);

		// reverse rotation
		rotate(p, 0, -ry);
		rotate(p, -rx, 0);

		m_test.push_back(p);
	}
}