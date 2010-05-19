#pragma once

#include <sdl.h>
#include <string>

namespace ufo
{
	using namespace std;

	class Rect : public SDL_Rect
	{
	public:

		Rect() { }
		Rect(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h);

		Rect& operator() (Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h);

		bool contains(Sint16 _x, Sint16 _y);
	};

	class Surface
	{
		SDL_Surface* m_surface;

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

		void setColors(SDL_Color* colors, int firstcolor, int ncolors);

		void loadSCR(string filename, Uint16 width = 320);

		void blit(Surface& surface, Rect* dst = NULL, Rect* src = NULL);

		void setClipRect(Rect& r);
		void clearClipRect();
	};
}