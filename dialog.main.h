#pragma once
#include "uidialog.h"
#include "font.h"

namespace ufo
{
	class LanguageDialog : public UIDialog
	{
		SmallFont m_smFont;

	public:

		LanguageDialog();

		void onCreate();
		void onOpen();
	};

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

	class DifficultyDialog : public UIDialog
	{
	};
}