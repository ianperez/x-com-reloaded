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

	short TrigTable::operator () (size_t i)
	{
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

				// transform points to spherical coordinates
				Point3d p1;
				toSpherical(m_map[i][j], p1);

				Point3d p2;
				toSpherical(m_map[i][k], p2);

				// perform basic rotation

				// perform basic translation

				// perform basic projection
				Point2d p3;
				p3.x = p1.x / p1.z;
				p3.y = p1.y / p1.z;

				Point2d p4;
				p4.x = p2.x / p2.z;
				p4.y = p2.y / p2.z;

				// move origin (0, 0) to center of screen
				p3.x += surface->w / 2;
				p3.y += surface->h / 2;
				p4.x += surface->w / 2;
				p4.y += surface->h / 2;

				lineRGBA(surface, p3.x, p3.y, p4.x, p4.y, 255, 255, 255, 255);
			}
		}
	}

	void WorldMap::toSpherical(const Point2d& p1, Point3d& p2)
	{
		p2.x = m_sin(p1.y + 720) * m_cos(p1.x + rx);
		p2.y = m_sin(p1.y + 720) * m_sin(p1.x + rx);
		p2.z = m_cos(p1.y + 720) + distance;
	}
}