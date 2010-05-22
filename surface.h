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
		Rect(const SDL_Rect& r);
		Rect(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h);

		Rect& operator() (Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h);

		bool contains(Sint16 _x, Sint16 _y);
	};

	class Surface
	{
		SDL_Surface* m_surface;

		Uint16 m_frameWidth;
		Uint16 m_frameHeight;
		Uint16 m_frames;

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

		Uint8 getPixel8(Sint16 x, Sint16 y);

		Uint32 mapRGB(Uint8 r, Uint8 g, Uint8 b);
		void setColors(SDL_Color* colors, int firstcolor, int ncolors);

		void loadSCR(string filename, Uint16 width = 320);

		void blit(Surface& surface, Rect* dst = NULL, Rect* src = NULL);

		// Frame functions
		void setFrames(Uint16 frameWidth, Uint16 frameHeight, Uint16 frames);
		Rect getFrameRect(Uint16 frame);
		Surface getFrameSurface(Uint16 frame);

		void invert(Uint8 pivot, Rect* r = NULL);

		void setClipRect(Rect& r);
		void clearClipRect();
	};
}