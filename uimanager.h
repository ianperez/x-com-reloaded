#pragma once
#include "uielement.h"

namespace ufo
{
	class UIManager : public UIElement
	{
	public:

		bool processEvent(SDL_Event& e);
		void draw(SDL_Surface* surface);
	};
}