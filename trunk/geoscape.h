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

	class AbandonGameDialog : public UIDialog
	{
		BigFont m_bgFont;

	public:

		AbandonGameDialog();

		void onCreate();
		void onOpen();

		void draw(Surface& surface);
	};

	class GeoScape : public UIElement
	{
	public:

		GeoScape(Globe::StartMode mode = Globe::Normal);

		void onCreate();

		void draw(Surface& surface);

	private:

		Globe::StartMode m_mode;

		Surface m_bg;
		Palette m_palette;

		Globe* m_globe;
	};
}