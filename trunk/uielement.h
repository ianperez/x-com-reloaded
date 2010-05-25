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
		virtual void onCreate() { }
		virtual void onDestroy() { }
	};

	class UIManager;

	class UIElement : public Rect, public UIEventHandler
	{
	public:

		friend class UIManager;

		UIElement();
		UIElement(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h);

		virtual void draw(Surface& surface) { }

		Uint16 getId() const { return m_id; }
		void setId(Uint16 id) { m_id = id; }

		void create(UIElement* e);

	protected:

		Uint16 m_id;
		UIManager* m_ui;

		vector<UIElement*> m_elements;
		UIElement* m_parent;

		Uint32 m_lastUpdate;
		Uint32 m_timeElapsed;

		bool m_exclusive;
	};
}