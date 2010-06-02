#pragma once
#include <string>
#include "uielement.h"
#include "palette.h"
#include "font.h"

namespace ufo
{
	using namespace std;

	class UIEdit : public UIElement
	{
	public:

		UIEdit(Font& font, Sint16 x, Sint16 y, Uint16 w, Uint8 size, bool editing = false, Uint8 color = Palette::blockSize * 8 + 6);

		void onCreate();

		bool onKeyDown(SDL_keysym keysym);
		bool onMouseLeftClick(Sint16 x, Sint16 y);

		bool onFocus();
		bool onBlur();

		virtual void onEnter() { }
		virtual void onCancel() { }

		void draw(Surface& surface);

	protected:

		Font& m_font;
		Uint8 m_color;

		string m_buffer;
		Uint8 m_size;

		bool m_editing;
	};
}