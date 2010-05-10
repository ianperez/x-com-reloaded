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
			vector<Uint32> colors;
			for (Uint16 i = 0; i < paletteSize; ++i)
			{
				Uint8 r, g, b;
				infile.read((char*)&r, sizeof(r));
				infile.read((char*)&g, sizeof(g));
				infile.read((char*)&b, sizeof(b));

				if (infile.eof())
					break;

				colors.push_back(GetColor((r + 1) * 4 - 1, (g + 1) * 4 - 1, (b + 1) * 4 - 1));
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

	Uint32 Palette::operator() (Uint8 index)
	{
		return m_colors[m_currentPalette][index - (m_colors[m_currentPalette].size() - 256)];
	}

	void Palette::setPalette(Uint8 palette)
	{
		m_currentPalette = palette;
	}
}