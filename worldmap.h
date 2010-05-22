#pragma once
#include <string>
#include <vector>
#include <map>
#include <sdl.h>
#include "uielement.h"
#include "palette.h"
#include "font.h"

namespace ufo
{
	using namespace std;

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

	class WorldMap : public UIElement
	{
		vector<GeoPolygon> m_world;
		vector<GeoObject> m_test;

		Point2d m_center;
		Sint16 m_rotx, m_rotz;

		Sint16 m_radius;
		Sint16 m_radiusMin;
		Sint16 m_radiusMax;
		vector<Sint16> m_zoomLevels;
		size_t m_zoom;

		Sint16 m_polarDegFix;

		Point2d m_defaultTarget;

		Surface m_bg;
		vector<Surface*> m_textures;
		Sint16 m_mx, m_my;

		Palette m_palette;

		void toCartesian(const Point2d& p1, Point3d& p2);
		void toSpherical(const Point3d& p1, Point2d& p2);
		bool screenToCartesian(Sint16 x, Sint16 y, Point3d& p);
		void rotate(Point3d& p, Sint16 x, Sint16 z);
		void project(const Point3d& p1, Point2d& p2);
		double distance(Point2d p1, Point2d p2);

		void drawShip(Surface& surface, Sint16 x, Sint16 y, Uint8 color);

	public:

		WorldMap(Surface& surface);

		void draw(Surface& surface);
		void center(Sint16 sx, Sint16 sy);
		void onClick(Sint16 sx, Sint16 sy);
		void rotateHorz(Sint16 delta);
		void rotateVert(Sint16 delta);
		void zoom(Sint8 delta);
		void setDefaultTarget(Sint16 sx, Sint16 sy);

		bool onMouseLeftClick(Sint16 x, Sint16 y);
		bool onMouseRightClick(Sint16 x, Sint16 y);
		bool onMouseHover(Sint16 x, Sint16 y);
		bool onKeyDown(SDL_keysym keysym);

		SmallFont m_font;
	};
}