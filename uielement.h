#pragma once
#include <vector>
#include <sdl.h>

namespace ufo
{
	using namespace std;

	class UIElement : public SDL_Rect
	{
	public:

		UIElement();

	protected:

		vector<UIElement *> m_elements;
	};
}