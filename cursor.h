#pragma once
#include <sdl.h>
#include "uielement.h"
#include "palette.h"

namespace ufo
{
	class Cursor : public UIElement
	{
		Uint8 m_color;
		Sint16 m_mousex, m_mousey;

	public:

		Cursor(Uint8 color = Palette::blockSize * 15 + 12);

		bool onMouseMove(Sint16 x, Sint16 y);
		void draw(Surface& surface);
	};
}