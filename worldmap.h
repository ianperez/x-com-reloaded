#pragma once
#include <string>
#include <vector>
#include <sdl.h>
#include "uielement.h"
#include "palette.h"
#include "font.h"

namespace ufo
{
	using namespace std;

	const long double Pi = 3.141592653589;

	struct Point2d
	{
		Point2d(Sint16 ix = 0, Sint16 iy = 0) : x(ix), y(iy) { }
		Sint16 x, y;
	};

	struct Point3d
	{
		Point3d(double ix = 0, double iy = 0, double iz = 0) : x(ix), y(iy), z(iz) { }
		double x, y, z;
	};

	struct GeoPoint
	{
		Point2d s; // spherical coordinate
		Point3d c; // cartesian coordinate
	};

	struct GeoObject : public GeoPoint
	{
		Uint32 lastUpdate;

		Point2d target;
	};

	struct GeoPolygon : public vector<GeoPoint>
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

	class WorldMap : public UIElement
	{
		Sin m_sin;
		Cos m_cos;

		vector<GeoPolygon> m_world;
		vector<GeoObject> m_test;

		Point2d m_center;

		Sint16 m_rotx, m_rotz;
		Sint16 m_radius;
		Sint16 m_radiusMin;
		Sint16 m_radiusMax;
		Sint16 m_polarDegFix;

		Point2d m_defaultTarget;

		SDL_Surface* m_bg;
		Point2d m_mouse;

		Palette m_palette;

		void toCartesian(const Point2d& p1, Point3d& p2);
		void toSpherical(const Point3d& p1, Point2d& p2);
		bool screenToCartesian(Sint16 x, Sint16 y, Point3d& p);
		void rotate(Point3d& p, Sint16 x, Sint16 z);
		void project(const Point3d& p1, Point2d& p2);
		Sint16 distance(Point2d p1, Point2d p2);

		void drawShip(SDL_Surface* surface, Sint16 x, Sint16 y, Uint32 color);

	public:

		WorldMap();

		void draw(SDL_Surface* surface);
		void center(Sint16 sx, Sint16 sy);
		void onClick(Sint16 sx, Sint16 sy);
		void rotateHorz(Sint16 delta);
		void rotateVert(Sint16 delta);
		void zoom(Sint16 delta);
		void setDefaultTarget(Sint16 sx, Sint16 sy);

		bool processEvent(SDL_Event& e);

		Font m_font;
	};
}