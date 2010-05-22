#pragma once
#include <string>
#include <vector>
#include <sdl.h>
#include <boost/shared_ptr.hpp>
#include "uielement.h"
#include "uibutton.h"
#include "palette.h"
#include "font.h"

namespace ufo
{
	using namespace std;
	using namespace boost;

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

	class GeoScape;

	class GeoScapeButton : public UIButton
	{
		GeoScape& m_gs;

	public:

		GeoScapeButton(GeoScape& gs, Sint16 _x, Sint16 _y, Sint16 _w, Sint16 _h, Uint16 id);

		void onPress();

		void draw(Surface& surface);

		// GeoScape button Id's
		enum {
			Intercept,
			Bases,
			Graphs,
			Ufopaedia,
			Options,
			Funding,
			Time5Sec,
			Time1Min,
			Time5Min,
			Time30Min,
			Time1Hour,
			Time1Day
		};
	};

	class GeoScape : public UIElement
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

		Surface m_bg;
		Surface m_surface;

		vector<shared_ptr<Surface> > m_textures;
		Point2d m_mouse;

		Palette m_palette;

		SmallFont m_font;

		void toCartesian(const Point2d& p1, Point3d& p2);
		void toSpherical(const Point3d& p1, Point2d& p2);
		bool screenToCartesian(Sint16 x, Sint16 y, Point3d& p);
		void rotate(Point3d& p, Sint16 x, Sint16 z);
		void project(const Point3d& p1, Point2d& p2);
		double distance(Point2d p1, Point2d p2);

		void drawShip(Surface& surface, Sint16 x, Sint16 y, Uint8 color);

	public:

		GeoScape(Surface& surface);

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

		void onCreate();
	};
}