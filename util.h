#pragma once
#include <sdl.h>
#include <string>
#include "palette.h"

namespace ufo
{
	using namespace std;

	const double Pi = 3.14159265358979;

	template<typename T>
	T round(double x)
	{
		double y;
		if (modf(x, &y) < 0.5)
			return static_cast<T>(floor(x));
		return static_cast<T>(ceil(x));
	}

	double toRad(double x);
	double toDeg(double x);

	void SDL_SetRect(SDL_Rect* r, Sint16 x, Sint16 y, Uint16 w, Uint16 h);
	bool SDL_PtInRect(SDL_Rect* r, Sint16 x, Sint16 y);
	Uint32 GetColor(Uint8 r, Uint8 g, Uint8 b);
	Uint32 GetColor(SDL_Color c);
	void pixelColor8(SDL_Surface* surface, Sint16 x, Sint16 y, Uint8 color);

	SDL_Surface* loadSCR(string filename, Uint16 width = 320);
}