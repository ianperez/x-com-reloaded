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
		double x, y, z;
	};

	struct Polygon2d : public vector<Point2d>
	{
		unsigned long texture;
	};

	class TrigTable
	{
	protected:

		vector<short> m_table;

		void load(string filename);

	public:

		short operator () (short i);
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

		vector<Polygon2d> m_map;

		void toSpherical(const Point2d& p1, Point3d& p2);
		double toRad(double deg);
		void rotate(Point3d& p);
		void project(SDL_Surface* surface, const Point3d& p1, Point2d& p2);

	public:

		WorldMap();

		short rx, ry;
		short radius;

		void draw(SDL_Surface* surface);
		void test(SDL_Surface* surface);
	};
}