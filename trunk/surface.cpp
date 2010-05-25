#include "surface.h"
#include <sdl_gfxprimitives.h>
#include <fstream>

using namespace std;

namespace ufo
{
	Rect::Rect(const SDL_Rect& r)
	{
		operator() (r.x, r.y, r.w, r.h);
	}

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

	void Rect::center(const Rect& r)
	{
		centerHorizontal(r);
		centerVertical(r);
	}

	void Rect::centerHorizontal(const Rect& r)
	{
		x = r.x + r.w / 2 - w / 2;
	}

	void Rect::centerVertical(const Rect& r)
	{
		y = r.y + r.h / 2 - h / 2;
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

	void Surface::setFrames(Uint16 frameWidth, Uint16 frameHeight, Uint16 frames)
	{
		m_frameWidth = frameWidth;
		m_frameHeight = frameHeight;
		m_frames = frames;
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
		if (x >= m_surface->clip_rect.x && y >= m_surface->clip_rect.y && x < m_surface->clip_rect.x + m_surface->clip_rect.w && y < m_surface->clip_rect.y + m_surface->clip_rect.h)
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

	void Surface::hollowRect(Rect* dst, Uint32 colorTopLeft, Uint32 colorBottomRight)
	{
		Rect r;

		// draw top line
		r.x = dst->x;
		r.y = dst->y;
		r.w = dst->w;
		r.h = 1;
		SDL_FillRect(m_surface, &r, colorTopLeft);

		// draw left line
		r.y = dst->y + 1;
		r.w = 1;
		r.h = dst->h - 1;
		SDL_FillRect(m_surface, &r, colorTopLeft);

		// draw bottom line
		r.x = dst->x + 1;
		r.y = dst->y + dst->h - 1;
		r.h = 1;
		r.w = dst->w - 1;
		SDL_FillRect(m_surface, &r, colorBottomRight);

		// draw right line
		r.x = dst->x + dst->w - 1;
		r.y = dst->y + 1;
		r.w = 1;
		r.h = dst->h - 2;
		SDL_FillRect(m_surface, &r, colorBottomRight);
	}

	Uint8 Surface::getPixel8(Sint16 x, Sint16 y)
	{
		lock();
		Uint8 c = *((Uint8 *)m_surface->pixels + y * m_surface->pitch + x);
		unlock();

		return c;
	}

	Uint32 Surface::mapRGB(Uint8 r, Uint8 g, Uint8 b)
	{
		return SDL_MapRGB(m_surface->format, r, g, b);
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
		if (m_surface && surface.m_surface)
			SDL_BlitSurface(m_surface, src, surface.m_surface, dst);
	}

	Rect Surface::getFrameRect(Uint16 frame)
	{
		Uint16 i = m_frameWidth * frame / w;
		return Rect(m_frameWidth * frame - w * i, i * m_frameHeight, m_frameWidth, m_frameHeight);
	}

	Surface Surface::getFrameSurface(Uint16 frame)
	{
		Surface surface(SDL_CreateRGBSurface(SDL_HWSURFACE, m_frameWidth, m_frameHeight, m_surface->format->BytesPerPixel * 8, 0, 0, 0, 0));
		Rect src(getFrameRect(frame));
		blit(surface, NULL, &src);

		return surface;
	}

	void Surface::invert(Uint8 pivot, Rect* r)
	{
		Rect cliprect(m_surface->clip_rect);
		if (!r)
			r = &cliprect;

		lock();

		for (Uint16 y = r->y; y < r->y + r->h; ++y)
		{
			Uint8* p = ((Uint8*)m_surface->pixels + y * m_surface->pitch + r->x);
			Uint8* end = p + r->w;
			while (p < end)
			{
				*p += 2 * (pivot - *p);
				++p;
			}
		}

		unlock();
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