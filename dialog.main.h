#pragma once
#include "uidialog.h"
#include "font.h"
#include "point.h"

namespace ufo
{
	class SaveLoadBase : public UIDialog
	{
	public:

		SaveLoadBase();

		void onCreate();
		void onOpen();
	};

	class SaveDialog : public SaveLoadBase
	{
	public:

		void onOpen();

		void draw(Surface& surface);
	};

	class LoadDialog : public SaveLoadBase
	{
	public:

		void onOpen();

		void draw(Surface& surface);
	};

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