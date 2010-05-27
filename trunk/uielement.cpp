#include "uielement.h"
#include "uimanager.h"

namespace ufo
{
	UIElement::UIElement()
		: m_exclusive(false), m_lastUpdate(SDL_GetTicks()), m_timeElapsed(0), m_parent(NULL), m_alwaysOnTop(false)
	{
	}

	UIElement::UIElement(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h)
		: m_exclusive(false), m_lastUpdate(SDL_GetTicks()), m_timeElapsed(0), m_parent(NULL), m_alwaysOnTop(false)
	{
		operator() (_x, _y, _w, _h);
	}

	void UIElement::create(UIElement* e)
	{
		e->m_parent = this;
		m_elements.push_back(e);
		m_ui->create(e);
	}
}