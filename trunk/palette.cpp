#include "palette.h"
#include "util.h"
#include <fstream>

using namespace std;

namespace ufo
{
	void Palette::load(string filename, Uint16 paletteSize)
	{
		ifstream infile(filename.c_str(), ios::binary);
		if (!infile)
			throw runtime_error("error opening " + filename);

		Uint8 bufferSize = paletteSize == 256 ? 6 : 0;

		vector<char> buffer(bufferSize);
		while (!infile.eof())
		{
			vector<SDL_Color> colors;
			for (Uint16 i = 0; i < paletteSize; ++i)
			{
				Uint8 r, g, b;
				infile.read((char*)&r, sizeof(r));
				infile.read((char*)&g, sizeof(g));
				infile.read((char*)&b, sizeof(b));

				if (infile.eof())
					break;

				SDL_Color c;
				c.r = (r + 1) * 4 - 1;
				c.g = (g + 1) * 4 - 1;
				c.b = (b + 1) * 4 - 1;
				c.unused = 0;
				colors.push_back(c);
			}

			m_colors.push_back(colors);

			if (bufferSize > 0)
				infile.read(&buffer[0], bufferSize);
		}
	}

	Palette::Palette()
		: m_currentPalette(0)
	{
	}

	Palette::Palette(string filename, Uint16 paletteSize)
		: m_currentPalette(0)
	{
		load(filename, paletteSize);
	}

	SDL_Color Palette::operator() (Uint8 index)
	{
		return m_colors[m_currentPalette][index + (256 - m_colors[m_currentPalette].size())];
	}

	void Palette::setPalette(Uint8 palette)
	{
		m_currentPalette = palette;
	}

	void Palette::apply(SDL_Surface* surface)
	{
		SDL_SetColors(surface, &m_colors[m_currentPalette][0], 256 - m_colors[m_currentPalette].size(), m_colors[m_currentPalette].size());
	}
}