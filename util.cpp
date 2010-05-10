#include "util.h"
#include <fstream>
#include <SDL_gfxPrimitives.h>

using namespace std;

namespace ufo
{
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

	SDL_Surface* loadSCR(string filename, Palette& p, Uint16 width)
	{
		ifstream infile(filename.c_str(), ios::binary);
		if (!infile)
			throw runtime_error("error opening " + filename);

		// calculate height
		infile.seekg(0, ios::end);
		Uint16 height = static_cast<Uint16>(infile.tellg() / width);
		infile.seekg(0, ios::beg);

		SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 24, 0, 0, 0, 0);
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
				pixelColor(surface, x, y, p(c));
			if (++x >= width)
			{
				x = 0;
				y++;
			}
		}

		return surface;
	}
}