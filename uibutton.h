#pragma once
#include <map>
#include "uielement.h"

namespace ufo
{
	using namespace std;

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

	class UIToggleButton : public UIButton
	{
	public:

		UIToggleButton(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h);

		virtual bool onMouseLeftClick(Sint16 x, Sint16 y);
		virtual bool onMouseLeftUnclick(Sint16 x, Sint16 y) { return true; }
	};

	class UIRadioButton : public UIButton
	{
	public:

		UIRadioButton(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h, Uint16 groupId);

		virtual bool onMouseLeftClick(Sint16 x, Sint16 y);
		virtual bool onMouseLeftUnclick(Sint16 x, Sint16 y) { return true; }

	protected:

		Uint16 m_groupId;
		static map<Uint16, UIRadioButton*> m_current;
	};
}