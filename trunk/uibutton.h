#pragma once
#include <map>
#include "uielement.h"
#include "font.h"

namespace ufo
{
	using namespace std;

	class UIPushButton : public UIElement
	{
	public:

		UIPushButton(Sint16 x, Sint16 y, Uint16 w, Uint16 h);

		virtual void onPress() { };
		virtual bool onMouseLeftClick(Sint16 x, Sint16 y);
		virtual bool onMouseLeftUnclick(Sint16 x, Sint16 y);

	protected:

		bool m_pressed;
	};

	class UIToggleButton : public UIPushButton
	{
	public:

		UIToggleButton(Sint16 x, Sint16 y, Uint16 w, Uint16 h);

		virtual bool onMouseLeftClick(Sint16 x, Sint16 y);
		virtual bool onMouseLeftUnclick(Sint16 x, Sint16 y) { return true; }
	};

	class UIRadioButton : public UIPushButton
	{
	public:

		UIRadioButton(Sint16 x, Sint16 y, Uint16 w, Uint16 h, Uint16 groupId);

		virtual bool onMouseLeftClick(Sint16 x, Sint16 y);
		virtual bool onMouseLeftUnclick(Sint16 x, Sint16 y) { return true; }

	protected:

		Uint16 m_groupId;
		static map<Uint16, UIRadioButton*> m_current;
	};

	class UIPushButtonStandard : public UIPushButton
	{
		string m_text;
		Font& m_font;

	public:

		UIPushButtonStandard(Font& font, string text, Sint16 x, Sint16 y, Uint16 w, Uint16 h);

		virtual void draw(Surface& surface);
	};
}