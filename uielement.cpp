#include "uielement.h"

namespace ufo
{
	UIElement::UIElement(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h)
		: exclusive(false)
	{
		operator() (_x, _y, _w, _h);
	}

	UIElement::~UIElement()
	{
		for (size_t i = 0; i < m_elements.size(); ++i)
			delete m_elements[i];
	}

	void UIElement::add(UIElement* element)
	{
		m_elements.push_back(element);
	}

	void UIElement::setUIManager(UIManager* ui)
	{
		m_ui = ui;
	}
}