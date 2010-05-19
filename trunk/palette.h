#pragma once
#include <sdl.h>
#include <string>
#include <vector>
#include "surface.h"

namespace ufo
{
	using namespace std;
	class Palette
	{
		void load(string filename, Uint16 paletteSize, Uint8 index);

	public:

		Palette();
		Palette(string filename, Uint16 paletteSize, Uint8 index);

		Uint32 getRGBA(Uint8 index) const;
		SDL_Color operator[] (Uint8 index) const;

		void apply(Surface& surface);

	protected:

		vector<SDL_Color> m_colors;
	};
}