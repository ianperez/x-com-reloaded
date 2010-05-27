#include "font.h"
#include "util.h"
#include "palette.h"
#include <fstream>

namespace ufo
{
	void Font::load(string filename)
	{
		ifstream infile(filename.c_str());
		if (!infile)
			throw runtime_error("error opening " + filename);

		infile.seekg(0, ios::end);
		m_data.resize(infile.tellg());
		infile.seekg(0, ios::beg);

		infile.read((char*)&m_data[0], m_data.size());
	}

	Font::Font(string filename, Uint16 width, Uint16 height)
		: m_width(width), m_height(height), m_color(Palette::blockSize * 8 + 6), m_spaceWidth(width / 2), m_charSpacing(0)
	{
		load(filename);
	}

	void Font::print(Surface& surface, Sint16 x, Sint16 y, string buffer, bool invert)
	{
		surface.lock();
		for (size_t i = 0; i < buffer.size(); ++i)
		{
			if (Uint8(buffer[i]) >= 33)
			{
				size_t j = m_width * m_height * (Uint8(buffer[i]) - 33);
				Uint16 max = 0;
				for (Uint16 k = 0; k < m_width * m_height; ++k)
				{
					if (j + k + 1 >= m_data.size())
						continue;

					Uint8 c = m_data[j + k];
					if (c > 0)
					{
						if (invert)
							c += 2 * (3 - c);

						surface.pixelColor8(x + (k % m_width), y + (k / m_width), c + m_color - 1);
						if ((k % m_width) > max)
							max = k % m_width;
					}
				}

				x += max + m_charSpacing;
			}
			else
				x += m_spaceWidth;
		}
		surface.unlock();
	}

	Uint16 Font::getTextWidth(string buffer)
	{
		Uint16 width = 0;

		for (size_t i = 0; i < buffer.size(); ++i)
		{
			if (buffer[i] >= 33)
			{
				size_t j = m_width * m_height * (buffer[i] - 33);
				Uint16 max = 0;
				for (Uint16 k = 0; k < m_width * m_height; ++k)
				{
					if (j + k + 1 >= m_data.size())
						continue;

					Uint8 c = m_data[j + k];
					if (c > 0 && (k % m_width) > max)
						max = k % m_width;
				}

				width += max + m_charSpacing;
			}
			else
				width += m_spaceWidth;
		}

		return width;
	}

	void Font::setColor(Uint8 color)
	{
		m_color = color;
	}

	SmallFont::SmallFont()
		: Font("geodata/smallset.dat", 8, 9)
	{
		m_spaceWidth = 5;
		m_charSpacing = 0;
	}

	BigFont::BigFont()
		: Font("geodata/biglets.dat", 16, 16)
	{
		m_spaceWidth = 10;
		m_charSpacing = 1;
	}

	void TextRenderer::print(Surface& surface, Rect& r, Uint16 stringId, FontType type, HorizontalAlignment halign, VerticalAlignment valign, bool invert)
	{
		Font* font = &m_smFont;
		if (type == BigFont)
			font = &m_bgFont;

		surface.setClipRect(r);

		Rect textRect(r.x, r.y, 0, 0);
		for (size_t i = 0; i < m_strings[stringId].size(); ++i)
		{
			if (i > 0)
				font = &m_smFont;

			textRect.w = font->getTextWidth(m_strings[stringId][i]);
			textRect.h = font->getHeight();

			if (halign == AlignCenter)
				textRect.centerHorizontal(r);
			else if (halign == AlignRight)
				textRect.x = r.x + r.w - textRect.w;

			if (valign == AlignMiddle)
				textRect.centerVertical(r);
			else if (valign == AlignBottom)
				textRect.y = r.y + r.h - textRect.h;

			font->setColor(m_color);
			font->print(surface, textRect.x, textRect.y, m_strings[stringId][i], invert);

			textRect.y += textRect.h + m_lineSpacing;
		}

		surface.clearClipRect();
	}
}