#pragma once
#include <sdl.h>
#include <string>
#include <vector>

namespace ufo
{
	using namespace std;
	class Palette
	{
		void load(string filename, Uint16 paletteSize);

	public:

		Palette();
		Palette(string filename, Uint16 paletteSize);

		SDL_Color operator() (Uint8 index);
		void setPalette(Uint8 palette);
		void apply(SDL_Surface* surface);

	protected:

		Uint8 m_currentPalette;
		vector<vector<SDL_Color> > m_colors;
	};
}