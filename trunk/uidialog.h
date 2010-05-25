#pragma once
#include "uielement.h"

namespace ufo
{
	class UIDialog : public UIElement
	{
		bool m_open;
		Uint16 m_openWidth;
		Uint16 m_openHeight;

	public:

		enum AnimationType {
			None,
			Horizontal,
			Vertical,
			Both
		};

		UIDialog(Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h, Uint8 color, AnimationType type);

		void center(Rect& r);
		virtual void draw(Surface& surface);

		virtual void onOpen() { }

	protected:

		Uint8 m_color;
		Surface m_bg;

		AnimationType m_type;

		double m_speed;
	};
}