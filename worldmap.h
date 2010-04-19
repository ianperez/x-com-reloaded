#pragma once
#include <string>
#include <vector>
#include <sdl.h>

namespace ufo
{
	using namespace std;

	struct Point2d
	{
		long x, y;
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

		short operator () (long i);
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
		void rotate(Point3d& p);
		void project(SDL_Surface* surface, const Point3d& p1, Point2d& p2);

	public:

		WorldMap();

		double distance;
		double rx, ry, rz;
		double radius;
		short lx, ly;

		void draw(SDL_Surface* surface);
		void test(SDL_Surface* surface);
	};
}