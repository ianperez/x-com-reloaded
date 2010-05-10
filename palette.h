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

		Uint32 operator() (Uint8 index);
		void setPalette(Uint8 palette);

	protected:

		Uint8 m_currentPalette;
		vector<vector<Uint32> > m_colors;
	};
}