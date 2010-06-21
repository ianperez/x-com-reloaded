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
#include "objectstate.h"

namespace ufo
{
	using namespace std;
	using namespace boost;
	using namespace Point;

	class GlobeState : public ObjectState
	{
	public:

		GlobeState();

		void save(string path);
		void load(string path);

		Sint16 rx, rz;
		Sint16 radius;
	};

	class GeoPolygon : public vector<GeoPoint>
	{
	public:

		bool contains(const Screen& p);

		Uint32 texture;
	};

	class Globe : public UIElement
	{
	public:

		enum StartMode
		{
			Normal,
			CreateBase
		};

		Globe(StartMode mode);

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

	private:

		vector<GeoPolygon> m_polygons;
		vector<GeoObject> m_test;

		Screen m_center;

		vector<Sint16> m_zoomLevels;
		vector<Sint16>::iterator m_zoom;

		Sint16 m_polarDegFix;

		GeoPoint m_defaultTarget;

		vector<shared_ptr<Surface> > m_textures;
		Screen m_mouse;

		Palette m_palette;

		SmallFont m_font;

		bool m_debug;

		StartMode m_mode;
		UIElement* m_newBaseDialog;

		vector<GeoPolygon>::iterator screenToPolygon(const Screen& p);
		void update();
		void drawShip(Surface& surface, Sint16 x, Sint16 y, Uint8 color);
	};
}