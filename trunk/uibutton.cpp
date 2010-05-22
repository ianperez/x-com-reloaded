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

	UIToggleButton::UIToggleButton(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h)
		: UIButton(_x, _y, _w, _h)
	{
	}

	bool UIToggleButton::onMouseLeftClick(Sint16 x, Sint16 y)
	{
		m_pressed = !m_pressed;
		return true;
	}

	map<Uint16, UIRadioButton*> UIRadioButton::m_current;

	UIRadioButton::UIRadioButton(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h, Uint16 groupId)
		: UIButton(_x, _y, _w, _h), m_groupId(groupId)
	{
	}

	bool UIRadioButton::onMouseLeftClick(Sint16 x, Sint16 y)
	{
		if (!m_pressed)
		{
			m_current[m_groupId]->m_pressed = false;
			m_current[m_groupId] = this;
			m_pressed = true;
			onPress();
		}

		return true;
	}
}