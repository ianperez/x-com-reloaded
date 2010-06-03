#pragma once
#include <string>
#include "point.h"
#include "surface.h"

namespace ufo
{
	using namespace std;

	class Base
	{
	public:

		Base();

		string getName() const { return m_name; }
		GeoPoint& getLocation() { return m_location; }

		void setLocation(GeoPoint p);
		void setName(string name);

		void draw(Surface& surface, Point2d p);

	protected:

		Uint16 m_detectShort;
		Uint16 m_detectLong;
		Uint16 m_detectHyper;

		Uint8 m_color;
		GeoPoint m_location;

		string m_name;
	};
}