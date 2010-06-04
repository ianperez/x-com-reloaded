#pragma once
#include <sdl.h>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace ufo
{
	using namespace std;
	using namespace boost;

	class Rect : public SDL_Rect
	{
	public:

		Rect() { }
		Rect(const SDL_Rect& r);
		Rect(Sint16 x, Sint16 y, Uint16 w, Uint16 h);

		Rect& operator() (Sint16 x, Sint16 y, Uint16 w, Uint16 h);

		bool contains(Sint16 x, Sint16 y);

		void center(const Rect& r);
		void centerHorizontal(const Rect& r);
		void centerVertical(const Rect& r);
	};

	class Surface
	{
		SDL_Surface* m_surface;

		Uint16 m_frameWidth;
		Uint16 m_frameHeight;
		Uint16 m_frames;

		Surface(const Surface& surface) { }

	public:

		Surface();
		Surface(SDL_Surface* surface);
		~Surface();

		Uint16 w, h;

		SDL_Surface* get();

		void lock();
		void unlock();
		void flip();

		void pixelColor8(Sint16 x, Sint16 y, Uint8 color);
		void lineColor8(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 color);
		void fillRect(Rect* dst, Uint32 color);
		void hollowRect(Rect* dst, Uint32 colorTopLeft, Uint32 colorBottomRight);

		Uint8 getPixel8(Sint16 x, Sint16 y);

		Uint32 mapRGB(Uint8 r, Uint8 g, Uint8 b);
		void setColors(SDL_Color* colors, int firstcolor, int ncolors);

		void loadSCR(string filename, Uint16 width = 320);

		void blit(Surface& surface, Rect* dst = NULL, Rect* src = NULL);

		// Frame functions
		void getFrames(vector<shared_ptr<Surface> >& v, Uint16 frameWidth, Uint16 frameHeight, Uint16 frames);

		void invert(Uint8 pivot, Rect* r = NULL);

		void setClipRect(Rect& r);
		void clearClipRect();
	};
}