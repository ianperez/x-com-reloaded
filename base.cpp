#include "base.h"

namespace ufo
{
	Base::Base()
		: m_color(9)
	{
	}

	void Base::setLocation(GeoPoint p)
	{
		m_location = p;
	}

	void Base::setName(string name)
	{
		m_name = name;
	}

	void Base::draw(Surface& surface, Point2d p)
	{
		Uint8 color = m_color + ((SDL_GetTicks() % 400) < 200 ? 1 : 0);

		Rect r(p.x - 1, p.y - 1, 3, 3);
		surface.hollowRect(&r, color, color);
	}
}