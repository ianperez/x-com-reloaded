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
				file.read((char*)&point.x, 2);
				if (file.eof())
					break;

				file.read((char*)&point.y, 2);
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
		transform();

		for (size_t i = 0; i < m_map.size(); ++i)
		{
			for (size_t j = 0; j < m_map[i].size(); ++j)
			{
				size_t k = (j + 1) % m_map[i].size();

				Point3d p1;
				toSpherical(m_map[i][j], p1);

				Point3d p2;
				Point2d p1;
				Point2d p2;

				p1.x = m_transformed[i][j].x / (m_transformed[i][j].z == 0 ? 1 : m_transformed[i][j].z);
				p1.y = m_transformed[i][j].y / (m_transformed[i][j].z == 0 ? 1 : m_transformed[i][j].z);

				p2.x = m_transformed[i][k].x / (m_transformed[i][k].z == 0 ? 1 : m_transformed[i][k].z);
				p2.y = m_transformed[i][k].y / (m_transformed[i][k].z == 0 ? 1 : m_transformed[i][k].z);

				p1.x += surface->w / 2;
				p1.y += surface->h / 2;
				p2.x += surface->w / 2;
				p2.y += surface->h / 2;

				lineRGBA(surface, p1.x, p1.y, p2.x, p2.y, 255, 255, 255, 255);
			}
		}
	}

	void WorldMap::toSpherical(Point2d& p1, Point3d& p2)
	{
		p2.x = m_sin(p1.y + 720) * m_cos(p1.x);
		p2.y = m_sin(p1.y + 720) * m_sin(p1.x);
		p2.z = m_cos(p1.y + 720);
	}
}