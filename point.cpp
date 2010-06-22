#include <cmath>
#include "point.h"
#include "util.h"

namespace ufo
{
	namespace Point
	{
		Screen::Screen(const Cartesian& c, const Screen& center)
		{
			x = round<Sint16>(c.x + center.x);
			y = round<Sint16>(center.y - c.z);
		}

		Spherical::Spherical(const Cartesian& c, Sint16 radius)
		{
			lat = round<Sint16>(toDeg(acos(c.z / radius)));
			lon = round<Sint16>(toDeg(atan2(c.y, c.x)));
		}

		double Spherical::distance(const Spherical& s) const
		{
			return sqrt(static_cast<double>((s.lon - lon) * (s.lon - lon) + (s.lat - lat) * (s.lat - lat)));
		}

		Spherical& Spherical::adjust(const Spherical& s)
		{
			if (distance(s) > Spherical(lon - 2880, lat).distance(s))
				lon -= 2880;
			else if (distance(s) > Spherical(lon + 2880, lat).distance(s))
				lon += 2880;

			return *this;
		}

		Cartesian::Cartesian(const Screen& s, const Screen& center, Sint16 radius, Sint16 rx, Sint16 rz)
			: m_invalid(false)
		{
			x = s.x - center.x;
			z = center.y - s.y;

			double ys = radius * radius - x * x - z * z;
			if (ys < 0)
				m_invalid = true;
			else
			{
				y = -sqrt(ys);

				rotate(-rx, 0);
				rotate(0, -rz);
			}
		}

		Cartesian::Cartesian(const Spherical& s, Sint16 radius)
			: m_invalid(false)
		{
			double rx = toRad(s.lon);
			double ry = toRad(s.lat);
			double sy = sin(ry);

			x = radius * sy * cos(rx);
			y = radius * sy * sin(rx);
			z = radius * cos(ry);
		}

		void Cartesian::rotate(Sint16 _x, Sint16 _z)
		{
			double rx = toRad(_x);
			double rz = toRad(_z);

			// rotate z-axis
			Cartesian orig(*this);
			y = cos(rz) * orig.y - sin(rz) * orig.x;
			x = sin(rz) * orig.y + cos(rz) * orig.x;

			// rotate x-axis
			orig = *this;
			z = cos(rx) * orig.z - sin(rx) * orig.y;
			y = sin(rx) * orig.z + cos(rx) * orig.y;
		}

		Cartesian::operator bool () const
		{
			return !m_invalid;
		}
	}

	void GeoPoint::sync(const Screen& center, Sint16 radius, Sint16 rx, Sint16 rz)
	{
		c = Cartesian(s, radius);
		c.rotate(rx, rz);
		visible = c.y <= 0;
		p = Screen(c, center);
	}
}