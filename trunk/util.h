#pragma once
#include <sdl.h>
#include <string>
#include "palette.h"

namespace ufo
{
	using namespace std;

	void SDL_SetRect(SDL_Rect* r, Sint16 x, Sint16 y, Uint16 w, Uint16 h);
	bool SDL_PtInRect(SDL_Rect* r, Sint16 x, Sint16 y);
	Uint32 GetColor(Uint8 r, Uint8 g, Uint8 b);

	SDL_Surface* loadSCR(string filename, Palette& p, Uint16 width = 320);
}