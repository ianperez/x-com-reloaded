#pragma once
#include <sdl.h>

namespace ufo
{
	void SDL_SetRect(SDL_Rect* r, Sint16 x, Sint16 y, Uint16 w, Uint16 h);
	Uint32 GetColor(Uint8 r, Uint8 g, Uint8 b);
}