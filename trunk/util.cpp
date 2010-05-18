#include "util.h"
#include <fstream>
#include <SDL_gfxPrimitives.h>
#include <cmath>

using namespace std;

namespace ufo
{
	double round(double x)
	{
		double y;
		if (modf(x, &y) < 0.5)
			return floor(x);
		return ceil(x);
	}

	double toRad(double x)
	{
		return x * Pi / 1440;
	}

	double toDeg(double x)
	{
		return x * 1440 / Pi;
	}

	void SDL_SetRect(SDL_Rect* r, Sint16 x, Sint16 y, Uint16 w, Uint16 h)
	{
		r->x = x;
		r->y = y;
		r->w = w;
		r->h = h;
	}

	bool SDL_PtInRect(SDL_Rect* r, Sint16 x, Sint16 y)
	{
		return (x >= r->x && y >= r->y && x <= r->x + r->w && y <= r->y + r->h);
	}

	Uint32 GetColor(Uint8 r, Uint8 g, Uint8 b)
	{
		return ((Uint32)r << 24) | (Uint32)(g << 16) | ((Uint16)b << 8) | (Uint8)255;
	}

	Uint32 GetColor(SDL_Color c)
	{
		return ((Uint32)c.r << 24) | (Uint32)(c.g << 16) | ((Uint16)c.b << 8) | (Uint8)255;
	}

	void pixelColor8(SDL_Surface* surface, Sint16 x, Sint16 y, Uint8 color)
	{
		if (SDL_MUSTLOCK(surface))
			SDL_LockSurface(surface);
		if (x >= 0 && y >= 0 && x < surface->w && y < surface->h)
			*((Uint8 *)surface->pixels + y * surface->pitch + x) = color;
		if (SDL_MUSTLOCK(surface))
			SDL_UnlockSurface(surface);
	}

	SDL_Surface* loadSCR(string filename, Uint16 width)
	{
		ifstream infile(filename.c_str(), ios::binary);
		if (!infile)
			throw runtime_error("error opening " + filename);

		// calculate height
		infile.seekg(0, ios::end);
		Uint16 height = static_cast<Uint16>(infile.tellg() / width);
		infile.seekg(0, ios::beg);

		SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 8, 0, 0, 0, 0);
		if (!surface)
			throw runtime_error("error creating surface for file: " + filename);

		Sint16 x = 0, y = 0;
		while (1)
		{
			Uint8 c;
			infile.read((char*)&c, sizeof(c));
			if (infile.eof())
				break;

			if (c > 0)
				pixelColor8(surface, x, y, c);
			if (++x >= width)
			{
				x = 0;
				y++;
			}
		}

		return surface;
	}
}