#pragma once
#include <string>
#include <vector>
#include <sdl.h>

namespace ufo
{
	using namespace std;

	struct Point2d
	{
		Sint16 x, y;
	};

	struct Point3d
	{
		double x, y, z;
	};

	struct Polygon3d : public vector<Point3d>
	{
		Uint32 texture;
	};

	class TrigTable
	{
	protected:

		vector<Sint16> m_table;

		void load(string filename);

	public:

		Sint16 operator () (Sint16 i);
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

		vector<Polygon3d> m_sphere;
		vector<Point3d> m_test;

		SDL_Surface* m_surface;

		void toSpherical(const Point2d& p1, Point3d& p2);
		void rotate(Point3d& p, Sint16 x, Sint16 y);
		void project(const Point3d& p1, Point2d& p2);

	public:

		WorldMap(SDL_Surface* m_surface);

		Sint16 rx, ry;
		Sint16 radius;

		void draw();
		void onClick(Sint16 sx, Sint16 sy);
	};
}