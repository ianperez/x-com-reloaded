#include "font.h"
#include "util.h"
#include "palette.h"
#include <fstream>

using namespace std;

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
		: m_width(width), m_height(height), m_color(Palette::blockSize * 8 + 6), m_spaceWidth(width / 2)
	{
		load(filename);
	}

	void Font::print(Surface& surface, Sint16 x, Sint16 y, string buffer, bool invert)
	{
		surface.lock();
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
					if (c > 0)
					{
						if (invert)
							c += 2 * (3 - c);

						surface.pixelColor8(x + (k % m_width), y + (k / m_width), c + m_color - 1);
						if ((k % m_width) > max)
							max = k % m_width;
					}
				}

				x += max;
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

				width += max;
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
		m_spaceWidth = 4;
	}

	BigFont::BigFont()
		: Font("geodata/biglets.dat", 16, 16)
	{
		m_spaceWidth = 12;
	}
}