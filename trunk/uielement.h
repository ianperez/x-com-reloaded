#pragma once
#include <vector>
#include <sdl.h>

namespace ufo
{
	using namespace std;

	class UIElement : public SDL_Rect
	{
	public:

		UIElement() { }
		UIElement(Sint16 ix, Sint16 iy, Uint16 iw, Uint16 ih);

		virtual bool processEvent(SDL_Event& e) { return false; }

	protected:

		vector<UIElement *> m_elements;
	};
}