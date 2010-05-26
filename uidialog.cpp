#include "uidialog.h"
#include "util.h"

namespace ufo
{
	UIDialog::UIDialog(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h, Uint8 color, AnimationType type)
		: UIElement(_x, _y, _w, _h), m_type(type), m_color(color), m_openWidth(_w), m_openHeight(_h), m_open(false), m_speed(2.0)
	{
		m_exclusive = true;
		const Uint16 min = 90;

		if (type == None)
		{
			m_open = true;
			onOpen();
		}
		if (type == Horizontal || type == Both)
		{
			x += w / 2 - min / 2;
			w = min;
		}
		if (type == Vertical || type == Both)
		{
			y += h / 2 - min / 2;
			h = min;
		}
	}

	void UIDialog::draw(Surface& surface)
	{
		// draw window border
		Rect r(*this);
		for (Sint8 i = 0; i < 5; ++i)
		{
			surface.fillRect(&r, m_color + abs(2 - i));
			r.x += 1;
			r.y += 1;
			r.w -= 2;
			r.h -= 2;
		}

		// draw background
		surface.setClipRect(r);
		m_bg.blit(surface);
		surface.clearClipRect();

		if (!m_open)
		{
			Uint16 delta = round<Uint16>(m_timeElapsed / m_speed);

			// update element size for animated opening
			if ((m_type == Horizontal || m_type == Both) && w < m_openWidth)
			{
				x -= delta;
				w += delta * 2;
			}
			if ((m_type == Vertical || m_type == Both) && h < m_openHeight)
			{
				y -= delta;
				h += delta * 2;
			}

			// adjust if we went past the open size
			if (w > m_openWidth)
			{
				x += (w - m_openWidth) / 2;
				w = m_openWidth;
			}
			if (h > m_openHeight)
			{
				y += (h - m_openHeight) / 2;
				h = m_openHeight;
			}

			if (!m_open && w == m_openWidth && h == m_openHeight)
			{
				m_open = true;
				onOpen();
			}
		}
	}
}