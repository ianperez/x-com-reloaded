#pragma once
#include "uielement.h"

namespace ufo
{
	class BaseView : public UIElement
	{
	public:

		BaseView();

		void onCreate();

		void draw(Surface& surface);
	};
}