#include "geoscape.h"
#include "uimanager.h"
#include "dialog.main.h"

namespace ufo
{
	class InterceptDialogButton : public UIPushButtonStandard
	{
	public:

		InterceptDialogButton(Uint16 stringId, Sint16 _x, Sint16 _y, Sint16 _w, Sint16 _h)
			: UIPushButtonStandard(stringId, _x, _y, _w, _h) { }

		void onCreate()
		{
			centerHorizontal(*m_parent);
		}

		void onPress()
		{
			m_ui->destroy(m_parent);
		}
	};

	InterceptDialog::InterceptDialog()
		: UIDialog(0, 0, 320, 140, Palette::blockSize * 15, UIDialog::Horizontal)
	{
	}

	void InterceptDialog::onCreate()
	{
		center(Rect(0, 0, m_ui->surface.w, m_ui->surface.h));
		m_bg.loadSCR("geograph/back12.scr");

		Palette p("geodata/backpals.dat", 4, 16);
		p.apply(m_bg);
		p.apply(m_ui->surface);
	}

	void InterceptDialog::onOpen()
	{
		create(new InterceptDialogButton(71, 0, y + h - 25, 288, 16));
	}

	void InterceptDialog::draw(Surface& surface)
	{
		UIDialog::draw(surface);

		if (m_open)
		{
			Rect r(0, y + 16, w, h);
			m_ui->text.setColor(Palette::blockSize * 15);
			m_ui->text.print(surface, r, 264, TextRenderer::BigFont, TextRenderer::AlignCenter);
		}
	}

	class OptionsDialogButton : public UIPushButtonStandard
	{
	public:

		OptionsDialogButton(Uint16 stringId, Sint16 _x, Sint16 _y, Sint16 _w, Sint16 _h)
			: UIPushButtonStandard(stringId, _x, _y, _w, _h) { }

		void onCreate()
		{
			centerHorizontal(*m_parent);
			m_color = Palette::blockSize * 15;
		}

		void onPress()
		{
			if (m_stringId == 800)
				m_ui->create(new AbandonGameDialog());

			m_ui->destroy(m_parent);
		}
	};

	OptionsDialog::OptionsDialog()
		: UIDialog(0, 20, 216, 160, Palette::blockSize * 15, UIDialog::Both)
	{
	}

	void OptionsDialog::onCreate()
	{
		centerHorizontal(Rect(0, 0, 256, 200));
		m_bg.loadSCR("geograph/back01.scr");

		Palette p("geodata/backpals.dat", 0, 16);
		p.apply(m_bg);
		p.apply(m_ui->surface);
	}

	void OptionsDialog::onOpen()
	{
		create(new OptionsDialogButton(316, 0, y + 40, 180, 20));
		create(new OptionsDialogButton(317, 0, y + 65, 180, 20));
		create(new OptionsDialogButton(800, 0, y + 90, 180, 20));
		create(new OptionsDialogButton(49, 0, y + 120, 180, 20));
	}

	void OptionsDialog::draw(Surface& surface)
	{
		UIDialog::draw(surface);

		if (m_open)
		{
			m_ui->text.setColor(Palette::blockSize * 15);
			m_ui->text.print(surface, Rect(x, y + 12, w, h), 315, TextRenderer::BigFont, TextRenderer::AlignCenter);
		}
	}

	class AbandonGameDialogButton : public UIPushButtonStandard
	{
	public:

		AbandonGameDialogButton(Uint16 stringId, Sint16 _x, Sint16 _y, Sint16 _w, Sint16 _h)
			: UIPushButtonStandard(stringId, _x, _y, _w, _h) { }

		void onCreate()
		{
			m_color = Palette::blockSize * 15;
		}

		void onPress()
		{
			if (m_stringId == 279)
			{
				m_ui->destroy(m_parent, true);
				m_ui->create(new MainMenuDialog());
			}
			else if (m_stringId == 280)
				m_ui->destroy(m_parent);
		}
	};

	AbandonGameDialog::AbandonGameDialog()
		: UIDialog(0, 20, 216, 160, Palette::blockSize * 15, UIDialog::Both)
	{
	}

	void AbandonGameDialog::onCreate()
	{
		centerHorizontal(Rect(0, 0, 256, 200));
		m_bg.loadSCR("geograph/back01.scr");

		Palette p("geodata/backpals.dat", 0, 16);
		p.apply(m_bg);
		p.apply(m_ui->surface);

		m_bgFont.setColor(Palette::blockSize * 15);
	}

	void AbandonGameDialog::onOpen()
	{
		create(new AbandonGameDialogButton(279, x + 18, y + 120, 50, 20));
		create(new AbandonGameDialogButton(280, x + w - 68, y + 120, 50, 20));
	}

	void AbandonGameDialog::draw(Surface& surface)
	{
		UIDialog::draw(surface);

		if (m_open)
		{
			string text(m_ui->strings(800) + '?');

			Rect r(x, y + 50, m_bgFont.getTextWidth(text), h);
			r.centerHorizontal(*this);
			m_bgFont.print(surface, r.x, r.y, text);
		}
	}

	class GeoScapeButton : public UIPushButton
	{
		Globe& m_globe;

	public:

		GeoScapeButton(Globe& globe, Sint16 _x, Sint16 _y, Uint16 id)
			: m_globe(globe), UIPushButton(_x, _y, 63, 11)
		{
			m_id = id;
		}

		// GeoScape button Id's
		enum {
			Intercept,
			Bases,
			Graphs,
			Ufopaedia,
			Options,
			Funding
		};

		void onPress()
		{
			if (m_id == Intercept)
				create(new InterceptDialog());
			else if (m_id == Options)
				create(new OptionsDialog());
		}

		void draw(Surface& surface)
		{
			if (m_pressed)
				surface.invert(248, this);
		}
	};

	class GeoScapeGlobeControl : public UIPushButton
	{
		Globe& m_globe;
		Point2d m_mouse;
		bool m_continuous;

	public:

		GeoScapeGlobeControl(Globe& globe, Sint16 _x, Sint16 _y, Uint16 _w, Uint16 _h, Uint16 id, bool continuous = true)
			: m_globe(globe), UIPushButton(_x, _y, _w, _h), m_continuous(continuous)
		{
			m_id = id;
		}

		// Map Control button Id's
		enum {
			RotateUp,
			RotateDown,
			RotateLeft,
			RotateRight,
			ZoomIn,
			ZoomOut
		};

		void onPress()
		{
			if (m_id == RotateUp)
				m_globe.rotateVertical(-Sint16(m_timeElapsed));
			if (m_id == RotateDown)
				m_globe.rotateVertical(m_timeElapsed);
			if (m_id == RotateLeft)
				m_globe.rotateHorizontal(-Sint16(m_timeElapsed));
			if (m_id == RotateRight)
				m_globe.rotateHorizontal(m_timeElapsed);
			if (m_id == ZoomIn)
				m_globe.zoom(1);
			if (m_id == ZoomOut)
				m_globe.zoom(-1);
		}

		bool onMouseLeftUnclick(Sint16 x, Sint16 y)
		{
			m_ui->releaseFocus(this);
			m_pressed = false;
			return true;
		}

		bool onMouseMove(Sint16 x, Sint16 y)
		{
			m_mouse.x = x;
			m_mouse.y = y;
			if (!m_pressed && m_continuous && (SDL_GetMouseState(0, 0) & SDL_BUTTON(1)))
			{
				m_ui->requestFocus(this);
				m_pressed = true;
			}
			return false;
		}

		bool onBlur()
		{
			m_pressed = false;
			return false;
		}

		void draw(Surface& surface)
		{
			if (m_pressed && contains(m_mouse.x, m_mouse.y))
			{
				onPress();
				if (!m_continuous)
					m_pressed = false;
			}
		}
	};

	class GeoScapeTimeButton : public UIRadioButton
	{
		Globe& m_globe;

	public:

		GeoScapeTimeButton(Globe& globe, Sint16 _x, Sint16 _y, Uint16 id)
			: m_globe(globe), UIRadioButton(_x, _y, 31, 13, 0)
		{
			m_id = id;
		}

		// GeoScape Time button Id's
		enum {
			Time5Sec,
			Time1Min,
			Time5Min,
			Time30Min,
			Time1Hour,
			Time1Day
		};

		void onPress()
		{
		}

		void draw(Surface& surface)
		{
			if (m_pressed)
				surface.invert(248, this);
		}
	};

	GeoScape::GeoScape()
		: UIElement(0, 0, 320, 200), m_palette("geodata/palettes.dat")
	{
	}

	void GeoScape::onCreate()
	{
		// load geoscape background image
		m_bg.loadSCR("geograph/geobord.scr");
		m_palette.apply(m_bg);

		// load geoscape panel overlay if not english
		if (m_ui->strings.getLanguage() != StringTable::English)
		{
			Surface overlay;
			overlay.loadSCR(m_ui->strings.getLanguage() == StringTable::German ? "geodata/lang2.dat" : "geodata/lang1.dat", 64);
			m_palette.apply(overlay);

			Rect overlayRect(m_bg.w - 64, 0, 0, 0);
			overlay.blit(m_bg, &overlayRect);
		}

		// apply palette to output surface
		m_palette.apply(m_ui->surface);

		// create globe
		m_globe = new Globe();
		create(m_globe);

		// create buttons
		create(new GeoScapeButton(*m_globe, 257, 0, GeoScapeButton::Intercept));
		create(new GeoScapeButton(*m_globe, 257, 12, GeoScapeButton::Bases));
		create(new GeoScapeButton(*m_globe, 257, 24, GeoScapeButton::Graphs));
		create(new GeoScapeButton(*m_globe, 257, 36, GeoScapeButton::Ufopaedia));
		create(new GeoScapeButton(*m_globe, 257, 48, GeoScapeButton::Options));
		create(new GeoScapeButton(*m_globe, 257, 60, GeoScapeButton::Funding));

		create(new GeoScapeTimeButton(*m_globe, 257, 112, GeoScapeTimeButton::Time5Sec));
		create(new GeoScapeTimeButton(*m_globe, 289, 112, GeoScapeTimeButton::Time1Min));
		create(new GeoScapeTimeButton(*m_globe, 257, 126, GeoScapeTimeButton::Time5Min));
		create(new GeoScapeTimeButton(*m_globe, 289, 126, GeoScapeTimeButton::Time30Min));
		create(new GeoScapeTimeButton(*m_globe, 257, 140, GeoScapeTimeButton::Time1Hour));
		create(new GeoScapeTimeButton(*m_globe, 289, 140, GeoScapeTimeButton::Time1Day));

		create(new GeoScapeGlobeControl(*m_globe, 271, 162, 13, 13, GeoScapeGlobeControl::RotateUp));
		create(new GeoScapeGlobeControl(*m_globe, 271, 186, 13, 13, GeoScapeGlobeControl::RotateDown));
		create(new GeoScapeGlobeControl(*m_globe, 259, 174, 13, 13, GeoScapeGlobeControl::RotateLeft));
		create(new GeoScapeGlobeControl(*m_globe, 284, 174, 13, 13, GeoScapeGlobeControl::RotateRight));
		create(new GeoScapeGlobeControl(*m_globe, 295, 155, 23, 23, GeoScapeGlobeControl::ZoomIn, false));
		create(new GeoScapeGlobeControl(*m_globe, 299, 182, 15, 17, GeoScapeGlobeControl::ZoomOut, false));
	}

	void GeoScape::draw(Surface& surface)
	{
		// draw background
		m_bg.blit(surface);
	}
}