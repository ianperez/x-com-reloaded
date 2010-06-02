#pragma once
#include <list>
#include "uielement.h"
#include "font.h"
#include "gamestate.h"

namespace ufo
{
	using namespace std;

	class Cursor;

	class UIManager : public list<UIElement*>
	{
		UIElement* m_focus;
		vector<UIElement*> m_toDestroy;

		bool processEvent(SDL_Event& e, UIElement* i);
		void updateTime(UIElement* e);

		bool m_debug;

		SmallFont m_font;

	public:

		UIManager(Surface& surface, StringTable& strings, GameState& state, Cursor* cursor);
		~UIManager();

		void create(UIElement* e);
		void destroy(UIElement* e, bool cascade = false);

		void cleanup();

		bool requestFocus(UIElement* e);
		bool releaseFocus(UIElement* e);

		bool processEvent(SDL_Event& e);

		void logic();
		void draw();

		Surface& surface;
		StringTable& strings;
		GameState& state;
		TextRenderer text;

		Cursor* cursor;
	};
}