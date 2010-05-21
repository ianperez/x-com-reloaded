#pragma once
#include "uielement.h"

namespace ufo
{
	class UIManager : protected vector<UIElement*>
	{
		UIElement* m_focus;

	public:

		UIManager();
		~UIManager();

		void add(UIElement* e);

		bool requestFocus(UIElement* e);

		bool processEvent(SDL_Event& e);
		void draw(Surface& surface);
	};
}