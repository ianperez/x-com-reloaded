#include "util.h"
#include <fstream>
#include <SDL_gfxPrimitives.h>

using namespace std;

namespace ufo
{
	double toRad(double x)
	{
		return x * Pi / 1440;
	}

	double toDeg(double x)
	{
		return x * 1440 / Pi;
	}

	Uint32 GetColor(Uint8 r, Uint8 g, Uint8 b)
	{
		return ((Uint32)r << 24) | (Uint32)(g << 16) | ((Uint16)b << 8) | (Uint8)255;
	}

	Uint32 GetColor(SDL_Color c)
	{
		return ((Uint32)c.r << 24) | (Uint32)(c.g << 16) | ((Uint16)c.b << 8) | (Uint8)255;
	}
}