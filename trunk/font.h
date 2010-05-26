#pragma once
#include <sdl.h>
#include <string>
#include <vector>
#include "palette.h"
#include "stringtable.h"

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
		Uint8 m_charSpacing;
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

	class TextRenderer
	{
		SmallFont m_smFont;
		BigFont m_bgFont;
		StringTable& m_strings;

		Uint16 m_lineSpacing;
		Uint8 m_color;

	public:

		TextRenderer(StringTable& strings) : m_lineSpacing(0), m_strings(strings), m_color(Palette::blockSize * 8 + 6) { }

		enum Alignment
		{
			AlignLeft,
			AlignCenter,
			AlignRight
		};

		enum FontType
		{
			SmallFont,
			BigFont
		};

		void setColor(Uint8 color) { m_color = color; }

		void print(Surface& surface, Rect& r, Uint16 stringId, FontType type = SmallFont, Alignment align = AlignLeft, bool invert = false);
	};
}