#include "palette.h"
#include "util.h"
#include <fstream>

using namespace std;

namespace ufo
{
	void Palette::load(string filename, Uint8 index, Uint16 paletteSize)
	{
		ifstream infile(filename.c_str(), ios::binary);
		if (!infile)
			throw runtime_error("error opening " + filename);

		Uint8 bufferSize = paletteSize == 256 ? 6 : 0;
		m_offset = paletteSize == 256 ? 0 : 224;

		infile.seekg((paletteSize + bufferSize) * index * 3, ios::beg);

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
			m_colors.push_back(c);
		}
	}

	Palette::Palette()
		: m_offset(0)
	{
	}

	Palette::Palette(string filename, Uint8 index, Uint16 paletteSize)
		: m_offset(0)
	{
		load(filename, index, paletteSize);
	}

	Uint32 Palette::getRGBA(Uint8 index) const
	{
		SDL_Color c(operator[] (index));
		return ((Uint32)c.r << 24) | (Uint32)(c.g << 16) | ((Uint16)c.b << 8) | (Uint8)255;
	}

	SDL_Color Palette::operator[] (Uint8 index) const
	{
		return m_colors[index - m_offset];
	}

	void Palette::apply(Surface& surface)
	{
		surface.setColors(&m_colors[0], m_offset, m_colors.size());
	}
}