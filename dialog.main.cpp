#include "dialog.main.h"
#include "palette.h"
#include "uimanager.h"
#include "uibutton.h"
#include "geoscape.h"

namespace ufo
{
	class LanguageButton : public UIPushButtonStandard
	{
	public:

		LanguageButton(Font& font, string text, Sint16 y, Uint16 id)
			: UIPushButtonStandard(font, text, 0, y, 192, 20) { m_id = id; }

		enum
		{
			English,
			Deutsche,
			Francais
		};

		void onCreate()
		{
			centerHorizontal(*m_parent);
		}

		void onPress()
		{
			if (m_id == English)
				m_ui->strings.load("geodata/english.dat");
			else if (m_id == Deutsche)
				m_ui->strings.load("geodata/german.dat");
			else if (m_id == Francais)
				m_ui->strings.load("geodata/french.dat");

			m_ui->destroy(m_parent);
			m_ui->create(new MainMenuDialog());
		}
	};

	LanguageDialog::LanguageDialog()
		: UIDialog(0, 0, 256, 160, Palette::blockSize * 8 + 6, UIDialog::Both)
	{
	}

	void LanguageDialog::onCreate()
	{
		center(Rect(0, 0, m_ui->surface.w, m_ui->surface.h));

		m_bg.loadSCR("geograph/back01.scr");

		Palette mainPalette("geodata/palettes.dat");
		mainPalette.apply(m_ui->surface);

		Palette backPalette("geodata/backpals.dat", 0, 16);
		backPalette.apply(m_bg);
		backPalette.apply(m_ui->surface);
	}

	void LanguageDialog::onOpen()
	{
		create(new LanguageButton(m_smFont, "ENGLISH", y + 70, LanguageButton::English));
		create(new LanguageButton(m_smFont, "DEUTSCHE", y + 98, LanguageButton::Deutsche));
		create(new LanguageButton(m_smFont, "FRANCAIS", y + 126, LanguageButton::Francais));
	}

	class MainMenuButton : public UIPushButtonStandard
	{
	public:

		MainMenuButton(Font& font, string text, Sint16 y, Uint16 id)
			: UIPushButtonStandard(font, text, 0, y, 192, 20) { m_id = id; }

		enum
		{
			New,
			Load,
			Quit
		};

		void onCreate()
		{
			centerHorizontal(*m_parent);
		}

		void onPress()
		{
			if (m_id == New)
			{
				m_ui->destroy(m_parent);
				m_ui->create(new GeoScape());
			}
			else if (m_id == Quit)
			{
				m_ui->destroy(m_parent);

				SDL_Event e;
				e.type = SDL_QUIT;
				SDL_PushEvent(&e);
			}
		}
	};

	MainMenuDialog::MainMenuDialog()
		: UIDialog(0, 0, 256, 160, Palette::blockSize * 8 + 6, UIDialog::Both)
	{
	}

	void MainMenuDialog::onCreate()
	{
		center(Rect(0, 0, m_ui->surface.w, m_ui->surface.h));

		m_bg.loadSCR("geograph/back01.scr");

		Palette mainPalette("geodata/palettes.dat");
		mainPalette.apply(m_ui->surface);

		Palette backPalette("geodata/backpals.dat", 0, 16);
		backPalette.apply(m_bg);
		backPalette.apply(m_ui->surface);
	}

	void MainMenuDialog::onOpen()
	{
		create(new MainMenuButton(m_smFont, m_ui->strings[780][0], y + 70, MainMenuButton::New));
		create(new MainMenuButton(m_smFont, m_ui->strings[781][0], y + 98, MainMenuButton::Load));
		create(new MainMenuButton(m_smFont, m_ui->strings[801][0], y + 126, MainMenuButton::Quit));
	}

	void MainMenuDialog::draw(Surface& surface)
	{
		UIDialog::draw(surface);

		if (m_open)
		{
			Rect r(x, y + 25, w, h);
			m_ui->text.setColor(Palette::blockSize * 8 + 11);
			m_ui->text.print(surface, r, 779, TextRenderer::BigFont, TextRenderer::AlignCenter);
		}
	}
}