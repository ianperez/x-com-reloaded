#pragma once
#include "uidialog.h"
#include "font.h"
#include "point.h"
#include "saveinfo.h"
#include <map>

namespace ufo
{
	using namespace std;

	class SaveLoadBase : public UIDialog
	{
		Palette m_prev;
		SmallFont m_font;

		Sint16 m_headerStringId;

		map<Uint8, SaveInfo> m_saves;

	public:

		SaveLoadBase(Sint16 headerStringId);

		void onCreate();
		void onDestroy();
		void onOpen();

		void draw(Surface& surface);
	};

	class SaveDialog : public SaveLoadBase
	{
	public:

		SaveDialog() : SaveLoadBase(791) { }
	};

	class LoadDialog : public SaveLoadBase
	{
	public:

		LoadDialog() : SaveLoadBase(790) { }
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