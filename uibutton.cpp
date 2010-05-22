#include "uibutton.h"
#include "uimanager.h"

namespace ufo
{
	UIButton::UIButton(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h)
		: m_pressed(false), UIElement(_x, _y, _w, _h)
	{
	}

	bool UIButton::onMouseLeftClick(Sint16 x, Sint16 y)
	{
		m_ui->requestFocus(this);
		m_pressed = true;
		return true;
	}

	bool UIButton::onMouseLeftUnclick(Sint16 x, Sint16 y)
	{
		if (m_pressed)
		{
			if (contains(x, y))
				onPress();

			m_pressed = false;
			m_ui->releaseFocus(this);
		}

		return true;
	}
}