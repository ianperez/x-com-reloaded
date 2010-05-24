#include "uielement.h"

namespace ufo
{
	UIElement::UIElement()
		: m_exclusive(false), m_lastUpdate(SDL_GetTicks()), m_timeElapsed(0)
	{
	}

	UIElement::UIElement(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h)
		: m_exclusive(false), m_lastUpdate(SDL_GetTicks()), m_timeElapsed(0)
	{
		operator() (_x, _y, _w, _h);
	}
}