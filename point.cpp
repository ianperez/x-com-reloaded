#include <cmath>
#include "point.h"
#include "util.h"

namespace ufo
{
	void Point2d::toCartesian(Point3d& p, Sint16 radius)
	{
		double rx = toRad(x);
		double ry = toRad(y);
		double sy = sin(ry);

		p.x = radius * sy * cos(rx);
		p.y = radius * sy * sin(rx);
		p.z = radius * cos(ry);
	}

	double Point2d::distance(const Point2d& p)
	{
		return sqrt(static_cast<double>((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y)));
	}

	void Point2d::adjust(const Point2d& p)
	{
		if (distance(p) > Point2d(x - 2880, y).distance(p))
			x -= 2880;
		if (distance(p) > Point2d(x + 2880, y).distance(p))
			x += 2880;
	}

	void Point3d::toSpherical(Point2d& p, Sint16 radius)
	{
		p.y = round<Sint16>(toDeg(acos(z / radius)));
		p.x = round<Sint16>(toDeg(atan2(y, x)));
	}

	void Point3d::rotate(Sint16 _x, Sint16 _z)
	{
		double rx = toRad(_x);
		double rz = toRad(_z);

		// rotate z-axis
		Point3d orig(*this);
		y = cos(rz) * orig.y - sin(rz) * orig.x;
		x = sin(rz) * orig.y + cos(rz) * orig.x;

		// rotate x-axis
		orig = *this;
		z = cos(rx) * orig.z - sin(rx) * orig.y;
		y = sin(rx) * orig.z + cos(rx) * orig.y;
	}
}