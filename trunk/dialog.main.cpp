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

	void MainMenuDialog::draw()
	{
	}
}