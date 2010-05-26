#pragma once
#include "uielement.h"

namespace ufo
{
	class UIDialog : public UIElement
	{
		Uint16 m_openWidth;
		Uint16 m_openHeight;

	public:

		enum AnimationType {
			None,
			Horizontal,
			Vertical,
			Both
		};

		UIDialog(Sint16 x, Sint16 y, Uint16 w, Uint16 h, Uint8 color, AnimationType type);

		virtual void draw(Surface& surface);

		virtual void onOpen() { }

	protected:

		Uint8 m_color;
		Surface m_bg;

		AnimationType m_type;
		bool m_open;

		double m_speed;
	};
}