#include "cursor.h"

namespace ufo
{
	Cursor::Cursor(Uint8 color)
		: UIElement(0, 0, 320, 200), m_color(color)
	{
		m_alwaysOnTop = true;
	}

	bool Cursor::onMouseMove(Sint16 _x, Sint16 _y)
	{
		m_mousex = _x;
		m_mousey = _y;
		return false;
	}

	void Cursor::draw(Surface& surface)
	{
		for (Uint8 i = 0, j = 13; i < 9; i++)
		{
			for (Uint8 k = 0; k < j; ++k)
				surface.pixelColor8(m_mousex + i, m_mousey + k + i, m_color + (k < i ? k : i));

			j -= j > 6 ? 2 : 1;
		}
	}
}