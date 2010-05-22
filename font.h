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

	public:

		Font() { }
		Font(string filename, Uint16 width, Uint16 height);

		void print(Surface& surface, Sint16 x, Sint16 y, string buffer);
		void printf(Surface& surface, Sint16 x, Sint16 y, string format, ...);
		void colorOffset(Uint8 colorOffset);

	protected:

		vector<Uint8> m_data;
		Uint16 m_width, m_height;
		Uint8 m_spaceWidth;
		Uint8 m_colorOffset;
	};

	class SmallFont : public Font
	{
	public:

		SmallFont();
	};

	class BigFont : public Font
	{
	public:

		BigFont();
	};
}