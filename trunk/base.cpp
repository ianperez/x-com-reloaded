#include "base.h"

namespace ufo
{
	Base::Base()
		: m_color(9)
	{
	}

	void Base::setName(string name)
	{
		m_name = name;
	}

	void Base::draw(Surface& surface)
	{
		if (m_location.visible)
		{
			Uint8 color = m_color + ((SDL_GetTicks() % 400) < 200 ? 1 : 0);

			Rect r(m_location.p.x - 1, m_location.p.y - 1, 3, 3);
			surface.hollowRect(&r, color, color);
		}
	}
}