#pragma once
#include <sdl.h>

namespace ufo
{
	class Point3d;

	class Point2d
	{
	public:

		Point2d(Sint16 _x = 0, Sint16 _y = 0)
			: x(_x), y(_y) { }

		// convert Spherical coordinates to Cartesian coordinates
		void toCartesian(Point3d& p, Sint16 radius);
		double distance(const Point2d& p);
		Point2d& adjust(const Point2d& p);

		Sint16 x, y;
	};

	class Point3d
	{
	public:

		Point3d(double _x = 0, double _y = 0, double _z = 0)
			: x(_x), y(_y), z(_z) { }

		// convert Cartesian coordinates to Spherical coordinates
		void toSpherical(Point2d& p, Sint16 radius);

		// perform x/z rotation
		void rotate(Sint16 x, Sint16 z);

		double x, y, z;
	};

	struct GeoPoint
	{
		Point2d s; // spherical coordinate
		Point3d c; // cartesian coordinate
	};

	struct GeoObject : public GeoPoint
	{
		Uint32 lastUpdate;

		Point2d target;
	};
}