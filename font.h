#pragma once
#include <sdl.h>
#include <string>
#include <vector>
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

		Uint16 getHeight() const { return m_height; }
		Uint16 getTextWidth(string buffer);

		void print(Surface& surface, Sint16 x, Sint16 y, string buffer, bool invert = false);
		void setColor(Uint8 color);

	protected:

		vector<Uint8> m_data;
		Uint16 m_width, m_height;
		Uint8 m_spaceWidth;
		Uint8 m_color;
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