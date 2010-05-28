#pragma once
#include <string>
#include <vector>
#include <sdl.h>
#include <boost/shared_ptr.hpp>
#include "uielement.h"
#include "uidialog.h"
#include "palette.h"
#include "font.h"
#include "point.h"

namespace ufo
{
	using namespace std;
	using namespace boost;

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

	class Globe : public UIElement
	{
		vector<GeoPolygon> m_polygons;
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

		vector<shared_ptr<Surface> > m_textures;
		Point2d m_mouse;

		Palette m_palette;

		SmallFont m_font;

		bool m_debug;

		bool screenToCartesian(Sint16 x, Sint16 y, Point3d& p);
		void rotate(Point3d& p, Sint16 x, Sint16 z);
		void project(const Point3d& p1, Point2d& p2);
		double distance(Point2d p1, Point2d p2);

		void drawShip(Surface& surface, Sint16 x, Sint16 y, Uint8 color);

	public:

		Globe();

		void center(Sint16 sx, Sint16 sy);
		void onClick(Sint16 sx, Sint16 sy);
		void rotateHorizontal(Sint16 delta);
		void rotateVertical(Sint16 delta);
		void zoom(Sint8 delta);
		void setDefaultTarget(Sint16 sx, Sint16 sy);

		bool onMouseLeftClick(Sint16 x, Sint16 y);
		bool onMouseRightClick(Sint16 x, Sint16 y);
		bool onMouseMove(Sint16 x, Sint16 y);
		bool onKeyDown(SDL_keysym keysym);

		void onCreate();

		void logic();
		void draw(Surface& surface);
	};
}