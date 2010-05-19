#include "font.h"
#include "util.h"
#include <SDL_gfxPrimitives.h>
#include <fstream>
#include <sstream>

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
		: m_width(width), m_height(height), m_offset(20)
	{
		load(filename);
	}

	void Font::print(Surface& surface, Sint16 x, Sint16 y, string buffer)
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
						surface.pixelColor8(x + (k % m_width), y + (k / m_width), c + m_offset);
						if ((k % m_width) > max)
							max = k % m_width;
					}
				}

				x += max;
			}
			else
				x += m_width / 2;
		}
		surface.unlock();
	}

	void Font::printf(Surface& surface, Sint16 x, Sint16 y, string format, ...)
	{
		va_list ap;
		va_start(ap, format);

		string buffer;
		for (size_t i = 0; i < format.size(); ++i)
		{
			if (format[i] == '%')
			{
				if (++i >= format.size())
					break;

				if (format[i] == 's')
					buffer += va_arg(ap, char*);
				else if (format[i] == 'z')
					buffer += va_arg(ap, string);
				else if (format[i] == 'd')
				{
					stringstream temp;
					temp << va_arg(ap, Sint32);
					buffer += temp.str();
				}
				else if (format[i] == 'f')
				{
					stringstream temp;
					temp << va_arg(ap, double);
					buffer += temp.str();
				}
				else if (format[i] == 'c')
					buffer += va_arg(ap, char);
				else if (format[i] == '%')
					break;

				continue;
			}

			buffer += format[i];
		}

		va_end(ap);
		print(surface, x, y, buffer);
	}

	void Font::offset(Uint8 offset)
	{
		m_offset = offset;
	}
}