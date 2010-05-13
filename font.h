#pragma once
#include <sdl.h>
#include <string>
#include <vector>
#include <cstdarg>
#include "palette.h"

namespace ufo
{
	using namespace std;

	class Font
	{
		void load(string filename);
		void _write(SDL_Surface* surface, Sint16 x, Sint16 y, string buffer);

	public:

		Font() { }
		Font(string filename, Uint16 width, Uint16 height);

		void write(SDL_Surface* surface, Sint16 x, Sint16 y, string format, ...);
		void palette(Palette* p);
		void offset(Uint8 offset);

	protected:

		vector<Uint8> m_data;
		Uint16 m_width, m_height;
		Uint8 m_offset;

		Palette* m_palette;
	};
}