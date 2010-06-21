#pragma once
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "point.h"
#include "surface.h"
#include "objectstate.h"

namespace ufo
{
	using namespace std;

	class Base
	{
	public:

		Base();

		string getName() const { return m_name; }
		GeoPoint& getLocation() { return m_location; }

		void setName(string name);

		void draw(Surface& surface);

	protected:

		Uint16 m_detectShort;
		Uint16 m_detectLong;
		Uint16 m_detectHyper;

		Uint8 m_color;
		GeoPoint m_location;

		string m_name;
	};

	class BaseState : public ObjectState, public vector<shared_ptr<Base> >
	{
	};
}