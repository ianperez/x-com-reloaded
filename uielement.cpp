#include "uielement.h"

namespace ufo
{
	UIElement::UIElement()
		: exclusive(false)
	{
	}

	UIElement::UIElement(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h)
		: exclusive(false)
	{
		operator() (_x, _y, _w, _h);
	}

	Uint16 UIElement::getId() const
	{
		return m_id;
	}

	void UIElement::setId(Uint16 id)
	{
		m_id = id;
	}

	void UIElement::setUIManager(UIManager* ui)
	{
		m_ui = ui;
	}
}