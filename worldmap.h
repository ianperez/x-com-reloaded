#pragma once
#include <string>
#include <vector>
#include <sdl.h>

namespace ufo
{
	using namespace std;

	struct Point2d
	{
		short x, y;
	};

	struct Point3d
	{
		long x, y, z;
	};

	struct Polygon2d : public vector<Point2d>
	{
		unsigned long texture;
	};

	struct Polygon3d : public vector<Point3d>
	{
		unsigned long texture;
	};

	class TrigTable
	{
	protected:

		vector<short> m_table;

	public:

		void load(string filename);

		short operator () (size_t i);
	};

	class Sin : public TrigTable
	{

	public:

		Sin();
	};

	class Cos : public TrigTable
	{
	public:

		Cos();
	};

	class WorldMap
	{
		Sin m_sin;
		Cos m_cos;

		vector<Polygon2d> m_raw;
		vector<Polygon3d> m_transformed;

	public:

		WorldMap();

		unsigned short radius;
		unsigned short distance;

		void draw(SDL_Surface* surface);
		void transform();
	};
}