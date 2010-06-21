#pragma once
#include <sdl.h>

namespace ufo
{
	namespace Point
	{
		class Cartesian;
		class Screen
		{
		public:

			Screen(Sint16 _x = 0, Sint16 _y = 0)
				: x(_x), y(_y) { }

			Screen(const Cartesian& c, const Screen& center);

			Sint16 x, y;
		};

		class Spherical
		{
		public:

			Spherical(Sint16 _lon = 0, Sint16 _lat = 0)
				: lon(_lon), lat(_lat) { }

			Spherical(const Cartesian& c, Sint16 radius);

			// calculate distance to another point
			double distance(const Spherical& s) const;

			// adjust this point to the closest equivalent of another
			Spherical& adjust(const Spherical& s);

			Sint16 lon, lat;
		};

		class Cartesian
		{
			bool m_invalid;

		public:

			Cartesian(double _x = 0, double _y = 0, double _z = 0)
				: x(_x), y(_y), z(_z) { }

			Cartesian(const Screen& s, const Screen& center, Sint16 radius, Sint16 rx, Sint16 rz);
			Cartesian(const Spherical& s, Sint16 radius);

			void rotate(Sint16 x, Sint16 z);

			operator bool () const;

			double x, y, z;
		};
	}

	using namespace Point;

	class GeoPoint
	{
	public:

		GeoPoint()
			: visible(true) { }

		void sync(const Screen& center, Sint16 radius, Sint16 rx, Sint16 rz);

		Spherical s;
		Cartesian c;
		Screen p;

		bool visible;
	};

	class GeoObject : public GeoPoint
	{
	public:

		Uint32 lastUpdate;

		Spherical target;
	};
}