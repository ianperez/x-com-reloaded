#include "dialog.main.h"
#include "palette.h"
#include "uimanager.h"
#include "uibutton.h"

namespace ufo
{
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
	}

	void MainMenuDialog::draw(Surface& surface)
	{
		UIDialog::draw(surface);

		if (m_open)
		{
			string text("UFO");
			Rect r(0, y + 30, m_bgFont.getTextWidth(text), m_bgFont.getHeight());
			r.centerHorizontal(*this);

			m_bgFont.setColor(Palette::blockSize * 5 + 10);
			m_bgFont.print(surface, r.x, r.y, "UFO");
		}
	}
}