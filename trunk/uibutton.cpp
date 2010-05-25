#include "uibutton.h"
#include "uimanager.h"
#include "palette.h"

namespace ufo
{
	UIPushButton::UIPushButton(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h)
		: m_pressed(false), UIElement(_x, _y, _w, _h)
	{
	}

	bool UIPushButton::onMouseLeftClick(Sint16 x, Sint16 y)
	{
		m_ui->requestFocus(this);
		m_pressed = true;
		return true;
	}

	bool UIPushButton::onMouseLeftUnclick(Sint16 x, Sint16 y)
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
		: UIPushButton(_x, _y, _w, _h)
	{
	}

	bool UIToggleButton::onMouseLeftClick(Sint16 x, Sint16 y)
	{
		m_pressed = !m_pressed;
		return true;
	}

	map<Uint16, UIRadioButton*> UIRadioButton::m_current;

	UIRadioButton::UIRadioButton(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h, Uint16 groupId)
		: UIPushButton(_x, _y, _w, _h), m_groupId(groupId)
	{
		if (!m_current.count(groupId))
		{
			m_current[groupId] = this;
			m_pressed = true;
			onPress();
		}
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

	UIPushButtonStandard::UIPushButtonStandard(Font& font, string text, Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h)
		: UIPushButton(_x, _y, _w, _h), m_font(font), m_text(text)
	{
	}

	void UIPushButtonStandard::draw(Surface& surface)
	{
		// draw outside border
		Rect r(*this);
		surface.hollowRect(&r, Palette::blockSize * 8 + 6, Palette::blockSize * 8 + 10);

		// draw inside border
		r.x += 1;
		r.y += 1;
		r.w -= 2;
		r.h -= 2;
		surface.hollowRect(&r, Palette::blockSize * 8 + 7, Palette::blockSize * 8 + 9);

		// draw inside rect
		r.x += 1;
		r.y += 1;
		r.w -= 2;
		r.h -= 2;
		surface.fillRect(&r, Palette::blockSize * 8 + 8);

		if (m_pressed)
			surface.invert(Palette::blockSize * 8 + 8, this);

		// draw text in center of button
		Rect textRect(0, 0, m_font.getTextWidth(m_text), m_font.getHeight());
		textRect.center(*this);
		m_font.print(surface, textRect.x, textRect.y, m_text, m_pressed);
	}
}