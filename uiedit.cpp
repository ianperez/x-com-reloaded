#include <cctype>
#include "uiedit.h"
#include "uimanager.h"

namespace ufo
{
	UIEdit::UIEdit(Font& font, Sint16 _x, Sint16 _y, Uint16 _w, Uint8 size, bool editing, Uint8 color)
		: UIElement(_x, _y, _w, font.getHeight()), m_font(font), m_size(size), m_editing(editing), m_color(color)
	{
	}

	void UIEdit::onCreate()
	{
		if (m_editing)
			m_ui->requestFocus(this);
	}

	bool UIEdit::onKeyDown(SDL_keysym keysym)
	{
		if (m_editing)
		{
			if (keysym.sym == SDLK_RETURN)
				m_ui->releaseFocus(this);
			else if (keysym.sym == SDLK_ESCAPE)
				onCancel();
			else if (keysym.sym == SDLK_BACKSPACE)
			{
				if (m_buffer.size())
					m_buffer.erase(m_buffer.size() - 1, 1);
			}
			else if (m_buffer.size() < m_size && m_font.getTextWidth(m_buffer) < w && keysym.sym >= 32 && keysym.sym <= m_font.getMaxChar())
			{
				if ((keysym.unicode & 0xFF80) == 0)
					m_buffer += static_cast<Uint8>(keysym.unicode);
			}

			return true;
		}

		return false;
	}

	bool UIEdit::onMouseLeftClick(Sint16 _x, Sint16 _y)
	{
		if (!m_editing)
			m_ui->requestFocus(this);
		return true;
	}

	bool UIEdit::onFocus()
	{
		SDL_EnableUNICODE(1);
		m_editing = true;
		return true;
	}

	bool UIEdit::onBlur()
	{
		SDL_EnableUNICODE(0);
		m_editing = false;
		onEnter();
		return false;
	}

	void UIEdit::draw(Surface& surface)
	{
		m_font.print(surface, x, y, m_buffer + (m_editing && (SDL_GetTicks() % 20) < 10 ? "*" : ""));
	}
}