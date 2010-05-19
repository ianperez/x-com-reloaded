#pragma once
#include <vector>
#include <sdl.h>
#include "surface.h"

namespace ufo
{
	using namespace std;

	class UIElement : public Rect
	{
	public:

		UIElement() { }
		UIElement(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h);

		~UIElement();

		virtual bool processEvent(SDL_Event& e) { return false; }
		virtual void draw(Surface& surface) { }

		void add(UIElement* element);

	protected:

		vector<UIElement*> m_elements;
		UIElement* m_parent;
	};
}