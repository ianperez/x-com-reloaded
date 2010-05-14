#include "uielement.h"

namespace ufo
{
	UIElement::UIElement(Sint16 ix, Sint16 iy, Uint16 iw, Uint16 ih)
	{
		x = ix;
		y = iy;
		w = iw;
		h = ih;
	}

	void UIElement::add(shared_ptr<UIElement> element)
	{
		m_elements.push_back(element);
	}
}