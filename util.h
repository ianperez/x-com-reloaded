#pragma once
#include <sdl.h>
#include <string>
#include <cmath>

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

	Uint32 GetColor(Uint8 r, Uint8 g, Uint8 b);
	Uint32 GetColor(SDL_Color c);
	string format(string format, ...);
}