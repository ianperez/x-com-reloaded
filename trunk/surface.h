#pragma once

#include <sdl.h>

namespace ufo
{
	class Surface
	{
		SDL_Surface* m_surface;

	public:

		Surface();

		~Surface();
	};
}