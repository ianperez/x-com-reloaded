#pragma once
#include "uielement.h"

namespace ufo
{
	class UIButton : public UIElement
	{
	public:

		UIButton(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h);

		virtual void onPress() { };
		virtual bool onMouseLeftClick(Sint16 x, Sint16 y);
		virtual bool onMouseLeftUnclick(Sint16 x, Sint16 y);

	protected:

		bool m_pressed;
	};
}