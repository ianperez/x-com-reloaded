#pragma once
#include "uidialog.h"
#include "font.h"

namespace ufo
{
	class MainMenuDialog : public UIDialog
	{
		BigFont m_bgFont;
		SmallFont m_smFont;

	public:

		MainMenuDialog();

		void onCreate();
		void onOpen();

		void draw(Surface& surface);
	};
}