#pragma once
#include <string>
#include "point.h"

namespace ufo
{
	using namespace std;

	class Base
	{
	public:

		Base();

		void draw(Surface& s);

	protected:

		Uint16 m_detectShort;
		Uint16 m_detectLong;
		Uint16 m_detectHyper;

		Point2d m_location;

		string m_name;
	};
}