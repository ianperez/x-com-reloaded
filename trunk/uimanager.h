#pragma once
#include "uielement.h"
#include <list>

namespace ufo
{
	using namespace std;

	class UIManager : public list<UIElement*>
	{
		UIElement* m_focus;
		vector<UIElement*> m_toDestroy;

		bool processEvent(SDL_Event& e, UIElement* i);
		void updateTime(UIElement* e);

	public:

		UIManager(Surface& s);
		~UIManager();

		void create(UIElement* e);
		void destroy(UIElement* e);

		bool requestFocus(UIElement* e);
		bool releaseFocus(UIElement* e);

		bool processEvent(SDL_Event& e);
		void draw();

		Surface& surface;
	};
}