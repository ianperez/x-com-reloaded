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
		void load(string filename, Uint8 index, Uint16 paletteSize);

	public:

		Palette();
		Palette(string filename, Uint8 index = 0, Uint16 paletteSize = 256);

		Uint32 getRGBA(Uint8 index) const;
		SDL_Color operator[] (Uint8 index) const;

		void apply(Surface& surface);
		void apply(Surface& surface, Uint8 offset);
		void draw(Surface& surface, Sint16 x, Sint16 y);

		static const Uint8 blockSize = 16;

	protected:

		vector<SDL_Color> m_colors;
		Uint8 m_offset;
	};
}