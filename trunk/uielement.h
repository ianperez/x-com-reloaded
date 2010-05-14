#pragma once
#include <vector>
#include <sdl.h>
#include <boost/shared_ptr.hpp>

namespace ufo
{
	using namespace std;
	using namespace boost;

	class UIElement : public SDL_Rect
	{
	public:

		UIElement() { }
		UIElement(Sint16 ix, Sint16 iy, Uint16 iw, Uint16 ih);

		virtual bool processEvent(SDL_Event& e) { return false; }
		virtual void draw(SDL_Surface* surface) { }

		void add(shared_ptr<UIElement> element);

	protected:

		vector<shared_ptr<UIElement> > m_elements;
		shared_ptr<UIElement> m_parent;
	};
}