#include "util.h"

namespace ufo
{
	void SDL_SetRect(SDL_Rect* r, Sint16 x, Sint16 y, Uint16 w, Uint16 h)
	{
		r->x = x;
		r->y = y;
		r->w = w;
		r->h = h;
	}

	Uint32 GetColor(Uint8 r, Uint8 g, Uint8 b)
	{
		return ((Uint32)r << 24) | (Uint32)(g << 16) | ((Uint16)b << 8) | (Uint8)255;
	}
}