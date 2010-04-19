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

		short v;
		while (1)
		{
			file.read((char*)&v, 2);
			if (file.gcount() == 0)
				break;

			m_table.push_back(v);
		}
	}

	short TrigTable::operator () (long i)
	{
		return m_table[abs(i) % m_table.size()];
	}

	Sin::Sin()
	{
		load("geodata/sine.dat");
	}

	Cos::Cos()
	{
		load("geodata/cos.dat");
	}

	WorldMap::WorldMap()
	{
		const string filename("geodata/world.dat");
		ifstream file(filename.c_str(), ios::binary);
		if (!file)
			throw runtime_error("error opening " + filename);

		while (1)
		{
			Point2d point;
			Polygon2d polygon;
			for (char i = 0; i < 4; ++i)
			{
				short x, y;
				file.read((char*)&x, 2);
				if (file.eof())
					break;

				file.read((char*)&y, 2);

				point.x = x;
				point.y = y;

				if (point.x != -1)
					polygon.push_back(point);
			}

			if (file.eof())
				break;

			file.read((char*)&polygon.texture, 4);
			m_map.push_back(polygon);
		}
	}

	void WorldMap::draw(SDL_Surface* surface)
	{
		for (size_t i = 0; i < m_map.size(); ++i)
		{
			for (size_t j = 0; j < m_map[i].size(); ++j)
			{
				size_t k = (j + 1) % m_map[i].size();

				// transform raw points (from world.dat) to spherical coordinates
				Point3d p1;
				toSpherical(m_map[i][j], p1);

				Point3d p2;
				toSpherical(m_map[i][k], p2);

				// perform x/y rotation
				rotate(p1);
				rotate(p2);

				// perform basic translation
				p1.z += distance;
				p2.z += distance;

				// perform basic projection
				if (p1.z < 1 || p2.z < 1)
					continue;

				Point2d p3;
				project(surface, p1, p3);

				Point2d p4;
				project(surface, p2, p4);

				lineRGBA(surface, p3.x, p3.y, p4.x, p4.y, 255, 255, 255, 255);
			}
		}
	}

	void WorldMap::test(SDL_Surface* surface)
	{
		ofstream file("test.txt");
		for (size_t i = 0; i < m_map.size(); ++i)
		{
			for (size_t j = 0; j < m_map[i].size(); ++j)
			{
				size_t k = (j + 1) % m_map[i].size();

				// transform raw points (from world.dat) to spherical coordinates
				Point3d p1;
				toSpherical(m_map[i][j], p1);

				Point3d p2;
				toSpherical(m_map[i][k], p2);

				file << p1.x << '\t' << p1.y << '\t' << p1.z << '\t';
				file << p2.x << '\t' << p2.y << '\t' << p2.z << endl;
			}
		}
	}

	void WorldMap::toSpherical(const Point2d& p1, Point3d& p2)
	{
		short sx = m_sin(p1.x + lx);
		short cx = m_cos(p1.x + lx);
		short sy = m_sin(p1.y + ly);
		short cy = m_cos(p1.y + ly);

		p2.x = radius * sy * cx;
		p2.y = radius * sy * sx;
		p2.z = radius * cx;
	}

	void WorldMap::rotate(Point3d& p)
	{
		Point3d orig;

		// rotate y-axis
		orig = p;
		p.x = sin(rx) * orig.z + cos(rx) * orig.x;
		p.z = cos(rx) * orig.z - sin(rx) * orig.x;

		// rotate x-axis
		orig = p;
		p.y = cos(ry) * orig.y - sin(ry) * orig.z;
		p.z = sin(ry) * orig.y + cos(ry) * orig.z;
	}

	void WorldMap::project(SDL_Surface* surface, const Point3d& p1, Point2d& p2)
	{
		p2.x = static_cast<short>(surface->w * p1.x / p1.z + surface->w / 2);
		p2.y = static_cast<short>(surface->w * p1.y / p1.z + surface->h / 2);
	}
}