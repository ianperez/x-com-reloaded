#pragma once
#include "uidialog.h"
#include "font.h"

namespace ufo
{
	class LanguageDialog : public UIDialog
	{
	public:

		LanguageDialog();

		void onCreate();
		void onOpen();
	};

	class MainMenuDialog : public UIDialog
	{
	public:

		MainMenuDialog();

		void onCreate();
		void onOpen();

		void draw(Surface& surface);
	};

	class DifficultyDialog : public UIDialog
	{
	public:

		DifficultyDialog();

		void onCreate();
		void onOpen();

		void draw(Surface& surface);
	};
}