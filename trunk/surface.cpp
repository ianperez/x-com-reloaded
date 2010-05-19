#include "surface.h"
#include <sdl_gfxprimitives.h>
#include <fstream>

using namespace std;

namespace ufo
{
	Rect::Rect(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h)
	{
		operator() (_x, _y, _w, _h);
	}

	Rect& Rect::operator() (Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h)
	{
		x = _x;
		y = _y;
		w = _w;
		h = _h;

		return *this;
	}

	bool Rect::contains(Sint16 _x, Sint16 _y)
	{
		return (_x >= x && _y >= y && _x <= x + w && _y <= y + h);
	}

	Surface::Surface()
		: m_surface(0), w(0), h(0)
	{
	}

	Surface::Surface(SDL_Surface* surface)
		: m_surface(surface), w(surface->w), h(surface->h)
	{
		
	}

	Surface::~Surface()
	{
		if (m_surface)
			SDL_FreeSurface(m_surface);
	}

	SDL_Surface* Surface::get()
	{
		return m_surface;
	}

	void Surface::setColors(SDL_Color* colors, int firstcolor, int ncolors)
	{
		SDL_SetColors(m_surface, colors, firstcolor, ncolors);
	}

	void Surface::lock()
	{
		SDL_LockSurface(m_surface);
	}

	void Surface::unlock()
	{
		SDL_UnlockSurface(m_surface);
	}

	void Surface::flip()
	{
		SDL_Flip(m_surface);
	}

	void Surface::pixelColor8(Sint16 x, Sint16 y, Uint8 color)
	{
		if (x >= 0 && y >= 0 && x < m_surface->w && y < m_surface->h)
			*((Uint8 *)m_surface->pixels + y * m_surface->pitch + x) = color;
	}

	void Surface::lineColor8(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 color)
	{
		SDL_Color c(m_surface->format->palette->colors[color]);
		lineColor(m_surface, x1, y1, x2, y2, ((Uint32)c.r << 24) | (Uint32)(c.g << 16) | ((Uint16)c.b << 8) | (Uint8)255);
	}

	void Surface::fillRect(Rect* dst, Uint32 color)
	{
		SDL_FillRect(m_surface, dst, color);
	}

	void Surface::loadSCR(string filename, Uint16 width)
	{
		if (m_surface)
			SDL_FreeSurface(m_surface);

		ifstream infile(filename.c_str(), ios::binary);
		if (!infile)
			throw runtime_error("error opening " + filename);

		// calculate height
		infile.seekg(0, ios::end);
		w = width;
		h = static_cast<Uint16>(infile.tellg() / w);
		infile.seekg(0, ios::beg);

		m_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 8, 0, 0, 0, 0);
		if (!m_surface)
			throw runtime_error("error creating surface for file: " + filename);

		Sint16 x = 0, y = 0;
		while (1)
		{
			Uint8 c;
			infile.read((char*)&c, sizeof(c));
			if (infile.eof())
				break;

			if (c > 0)
				pixelColor8(x, y, c);
			if (++x >= width)
			{
				x = 0;
				y++;
			}
		}
	}

	void Surface::blit(Surface& surface, Rect* dst, Rect* src)
	{
		SDL_BlitSurface(m_surface, src, surface.m_surface, dst);
	}

	void Surface::setClipRect(Rect& r)
	{
		SDL_SetClipRect(m_surface, &r);
	}

	void Surface::clearClipRect()
	{
		SDL_SetClipRect(m_surface, NULL);
	}
}