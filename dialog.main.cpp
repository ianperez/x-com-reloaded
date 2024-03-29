#include "dialog.main.h"
#include "palette.h"
#include "uimanager.h"
#include "uibutton.h"
#include "geoscape.h"
#include "util.h"

namespace ufo
{
	class SaveLoadButton : public UIPushButtonStandard
	{
		SmallFont m_font;

	public:

		SaveLoadButton(Uint16 stringId, Sint16 x, Sint16 y, Uint16 w, Uint16 h, Uint16 id)
			: UIPushButtonStandard(stringId, x, y, w, h) { m_id = id; }

		void onCreate()
		{
			if (m_id == 0)
				centerHorizontal(*m_parent);
		}

		void onPress()
		{
			m_ui->destroy(m_parent);
		}

		void draw(Surface& surface)
		{
			UIPushButtonStandard::draw(surface);

			if (m_id > 0)
			{
				string text(format("%d", m_id));
				Rect r(0, 0, m_font.getTextWidth(text), m_font.getHeight());
				r.center(*this);
				m_font.print(surface, r.x, r.y, text);
			}
		}
	};

	SaveLoadBase::SaveLoadBase(Sint16 headerStringId)
		: UIDialog(0, 0, 320, 200, Palette::blockSize * 8 + 6, UIDialog::Both), m_headerStringId(headerStringId)
	{
	}

	void SaveLoadBase::onCreate()
	{
		m_prev = Palette(m_ui->surface);

		m_bg.loadSCR("geograph/back01.scr");
		Palette p("geodata/backpals.dat", 6, 16);
		p.apply(m_bg);
		p.apply(m_ui->surface);

		// load save info
		for (Uint8 i = 1; i <= 10; ++i)
		{
			string path(format("game_%d/saveinfo.dat", i));
			if (exists(path))
				m_saves[i] = SaveInfo(i);
		}
	}

	void SaveLoadBase::onDestroy()
	{
		m_prev.apply(m_ui->surface);
	}

	void SaveLoadBase::onOpen()
	{
		create(new SaveLoadButton(49, 0, 172, 80, 16, 0));
		for (size_t i = 0; i < 10; ++i)
			create(new SaveLoadButton(830, 10, 32 + i * 14, 24, 12, i + 1));
	}

	void SaveLoadBase::draw(Surface& surface)
	{
		UIDialog::draw(surface);
		m_ui->text.setColor(Palette::blockSize * 15);
		m_ui->text.print(surface, Rect(0, 8, 320, 200), m_headerStringId, TextRenderer::BigFont, TextRenderer::AlignCenter);

		for (map<Uint8, SaveInfo>::iterator i = m_saves.begin(); i != m_saves.end(); ++i)
		{
			m_font.setColor(Palette::blockSize * 8 + 11);
			m_font.print(surface, 40, 34 + i->first * 14, i->second.getName());
		}
	}

	class LanguageButton : public UIPushButtonStandard
	{
	public:

		LanguageButton(Uint16 stringId, Sint16 y, Uint16 id)
			: UIPushButtonStandard(stringId, 0, y, 192, 20) { m_id = id; }

		void onCreate()
		{
			centerHorizontal(*m_parent);
		}

		void onPress()
		{
			if (m_id == StringTable::English)
			{
				m_ui->strings.load("geodata/english.dat");
				m_ui->strings.setLanguage(StringTable::English);
			}
			else if (m_id == StringTable::German)
			{
				m_ui->strings.load("geodata/german.dat");
				m_ui->strings.setLanguage(StringTable::German);
			}
			else if (m_id == StringTable::French)
			{
				m_ui->strings.load("geodata/french.dat");
				m_ui->strings.setLanguage(StringTable::French);
			}

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
		create(new LanguageButton(0, y + 70, StringTable::English));
		create(new LanguageButton(1, y + 98, StringTable::German));
		create(new LanguageButton(2, y + 126, StringTable::French));
	}

	class MainMenuButton : public UIPushButtonStandard
	{
	public:

		MainMenuButton(Uint16 stringId, Sint16 y)
			: UIPushButtonStandard(stringId, 0, y, 192, 20) { }

		void onCreate()
		{
			centerHorizontal(*m_parent);
		}

		void onPress()
		{
			if (m_stringId == 780)
			{
				m_ui->destroy(m_parent);
				m_ui->create(new DifficultyDialog());
			}
			else if (m_stringId == 781)
				m_ui->create(new LoadDialog());
			else if (m_stringId == 801)
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

		// reset game state
		m_ui->state = GameState();
	}

	void MainMenuDialog::onOpen()
	{
		create(new MainMenuButton(780, y + 70));
		create(new MainMenuButton(781, y + 98));
		create(new MainMenuButton(801, y + 126));
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

	class DifficultyButton : public UIPushButtonStandard
	{
	public:

		DifficultyButton(Uint16 stringId, Sint16 y, Uint16 id)
			: UIPushButtonStandard(stringId, 0, y, 160, 18) { m_id = id; }

		void onCreate()
		{
			centerHorizontal(*m_parent);
		}

		enum
		{
			Beginner,
			Experienced,
			Veteran,
			Genius,
			Superhuman
		};

		void onPress()
		{
			m_ui->destroy(m_parent);
			m_ui->create(new GeoScape(Globe::CreateBase));
		}
	};

	DifficultyDialog::DifficultyDialog()
		: UIDialog(0, 0, 192, 180, Palette::blockSize * 8 + 6, UIDialog::Vertical)
	{
	}

	void DifficultyDialog::onCreate()
	{
		center(Rect(0, 0, m_ui->surface.w, m_ui->surface.h));

		m_bg.loadSCR("geograph/back01.scr");

		Palette mainPalette("geodata/palettes.dat");
		mainPalette.apply(m_ui->surface);

		Palette backPalette("geodata/backpals.dat", 0, 16);
		backPalette.apply(m_bg);
		backPalette.apply(m_ui->surface);
	}

	void DifficultyDialog::onOpen()
	{
		create(new DifficultyButton(783, y + 45, DifficultyButton::Beginner));
		create(new DifficultyButton(784, y + 70, DifficultyButton::Experienced));
		create(new DifficultyButton(785, y + 95, DifficultyButton::Veteran));
		create(new DifficultyButton(786, y + 120, DifficultyButton::Genius));
		create(new DifficultyButton(787, y + 145, DifficultyButton::Superhuman));
	}

	void DifficultyDialog::draw(Surface& surface)
	{
		UIDialog::draw(surface);

		if (m_open)
		{
			Rect r(x, y + 20, w, h);
			m_ui->text.setColor(Palette::blockSize * 8 + 11);
			m_ui->text.print(surface, r, 782, TextRenderer::SmallFont, TextRenderer::AlignCenter);
		}
	}
}