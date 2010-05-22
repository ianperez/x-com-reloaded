#pragma once
#include <vector>
#include <sdl.h>
#include "surface.h"

namespace ufo
{
	using namespace std;

	class UIEventHandler
	{
	public:

		// Mouse events
		virtual bool onMouseLeftClick(Sint16 x, Sint16 y) { return false; }
		virtual bool onMouseLeftUnclick(Sint16 x, Sint16 y) { return false; }
		virtual bool onMouseRightClick(Sint16 x, Sint16 y) { return false; }
		virtual bool onMouseRightUnclick(Sint16 x, Sint16 y) { return false; }
		virtual bool onMouseHover(Sint16 x, Sint16 y) { return false; }

		// Keyboard events
		virtual bool onKeyDown(SDL_keysym keysym) { return false; }
		virtual bool onKeyUp(SDL_keysym keysym) { return false; }

		// Focus events
		virtual bool onFocus() { return false; }
		virtual bool onBlur() { return false; }

		// Other events
		virtual bool onCreate() { return false; }
		virtual bool onDestroy() { return false; }
	};

	class UIManager;

	class UIElement : public Rect, public UIEventHandler
	{
	public:

		bool exclusive;

		UIElement();
		UIElement(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h);

		~UIElement();

		virtual void draw(Surface& surface) { }

		void add(UIElement* element);
		void setUIManager(UIManager* ui);

	protected:

		vector<UIElement*> m_elements;
		UIEventHandler* m_handler;
		UIManager* m_ui;
	};
}