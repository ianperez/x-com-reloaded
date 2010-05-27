#pragma once
#include <string>
#include <vector>
#include <sdl.h>
#include <boost/shared_ptr.hpp>
#include "uielement.h"
#include "uibutton.h"
#include "uidialog.h"
#include "palette.h"
#include "font.h"
#include "globe.h"

namespace ufo
{
	using namespace std;
	using namespace boost;

	class GeoScape;

	class InterceptDialog : public UIDialog
	{
	public:

		InterceptDialog();

		void onCreate();
		void onOpen();

		void draw(Surface& surface);
	};

	class OptionsDialog : public UIDialog
	{
	public:

		OptionsDialog();

		void onCreate();
		void onOpen();

		void draw(Surface& surface);
	};

	class GeoScape : public UIElement
	{
		Surface m_bg;
		Palette m_palette;

		Globe* m_globe;

	public:

		GeoScape();

		void onCreate();

		void draw(Surface& surface);
	};
}