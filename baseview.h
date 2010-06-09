#pragma once
#include "uielement.h"
#include "font.h"

namespace ufo
{
	class BaseView : public UIElement
	{
		SmallFont m_smFont;
		BigFont m_bgFont;

	public:

		BaseView();

		void onCreate();

		void draw(Surface& surface);
	};
}