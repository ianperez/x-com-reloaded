#include "util.h"
#include <cstdarg>
#include <sstream>

namespace ufo
{
	double toRad(double x)
	{
		return x * Pi / 1440;
	}

	double toDeg(double x)
	{
		return x * 1440 / Pi;
	}

	Uint32 GetColor(Uint8 r, Uint8 g, Uint8 b)
	{
		return ((Uint32)r << 24) | (Uint32)(g << 16) | ((Uint16)b << 8) | (Uint8)255;
	}

	Uint32 GetColor(SDL_Color c)
	{
		return ((Uint32)c.r << 24) | (Uint32)(c.g << 16) | ((Uint16)c.b << 8) | (Uint8)255;
	}

	string format(string format, ...)
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

		return buffer;
	}
}