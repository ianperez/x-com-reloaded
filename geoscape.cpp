#include "geoscape.h"
#include "uimanager.h"
#include "dialog.main.h"
#include "baseview.h"
#include "util.h"

namespace ufo
{
	class NewBaseDialogButton : public UIPushButtonStandard
	{
	public:

		NewBaseDialogButton()
			: UIPushButtonStandard(49, 186, 8, 54, 12) { }

		void onPress()
		{
			m_ui->destroy(m_parent);
		}
	};

	class NewBaseDialog : public UIDialog
	{
		bool m_showCancel;

	public:

		NewBaseDialog(bool showCancel = false)
			: UIDialog(0, 0, 256, 28, Palette::blockSize * 8 + 6), m_showCancel(showCancel) { }

		void onCreate()
		{
			m_exclusive = false;

			m_bg.loadSCR("geograph/back01.scr");

			Palette backPalette("geodata/backpals.dat", 0, 16);
			backPalette.apply(m_bg);
			backPalette.apply(m_ui->surface);

			// create dummy button to block geoscape buttons on the right
			create(new UIPushButton(256, 0, 64, 154));

			if (m_showCancel)
				create(new NewBaseDialogButton());

			m_ui->state.time.pause();
		}

		void onDestroy()
		{
			m_ui->state.time.pause(false);
		}

		void draw(Surface& surface)
		{
			UIDialog::draw(surface);
			m_ui->text.print(surface, Rect(8, 10, w, h), 283, TextRenderer::SmallFont);
		}
	};

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
				m_parent->create(new InterceptDialog());
			else if (m_id == Options)
				m_parent->create(new OptionsDialog());
			else if (m_id == Bases)
			{
				m_ui->destroy(m_parent);
				m_ui->create(new BaseView());
			}
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

		void onPress()
		{
			if (m_id == GameTime::Time5Sec)
				m_ui->state.time.setStep(GameTime::Time5Sec);
			else if (m_id == GameTime::Time1Min)
				m_ui->state.time.setStep(GameTime::Time1Min);
			else if (m_id == GameTime::Time5Min)
				m_ui->state.time.setStep(GameTime::Time5Min);
			else if (m_id == GameTime::Time30Min)
				m_ui->state.time.setStep(GameTime::Time30Min);
			else if (m_id == GameTime::Time1Hour)
				m_ui->state.time.setStep(GameTime::Time1Hour);
			else if (m_id == GameTime::Time1Day)
				m_ui->state.time.setStep(GameTime::Time1Day);
		}

		void draw(Surface& surface)
		{
			if (m_pressed)
				surface.invert(248, this);
		}
	};

	class GameTimeDisplay : public UIElement
	{
		SmallFont m_smFont;
		BigFont m_bgFont;

	public:

		GameTimeDisplay()
			: UIElement(259, 74, 59, 35)
		{
			m_smFont.setColor(Palette::blockSize * 15 + 5);
			m_bgFont.setColor(Palette::blockSize * 15 + 5);
		}

		void logic()
		{
			m_ui->state.time.increment(m_timeElapsed);
		}

		void draw(Surface& surface)
		{
			// draw time
			m_bgFont.print(surface, x + 20, y, ":  :");

			string hour(format("%d", m_ui->state.time.getHour()));
			m_bgFont.print(surface, x + (m_ui->state.time.getHour() > 9 ? 0 : 20 - m_bgFont.getTextWidth(hour)), y, hour);

			string minute(format("%.2d", m_ui->state.time.getMinute()));
			m_bgFont.print(surface, x + 24, y, minute);

			string second(format("%.2d", m_ui->state.time.getSecond()));
			m_smFont.print(surface, x + 48, y + 6, second);

			// draw week day
			string weekday(m_ui->strings(185 + m_ui->state.time.getWeekDay()));
			Rect textRect(x, y + 13, m_smFont.getTextWidth(weekday), h);
			textRect.centerHorizontal(*this);

			m_smFont.print(surface, textRect.x, textRect.y, weekday);

			// draw day
			string day(format("%d", m_ui->state.time.getDay() + 1));
			Uint8 index = day[day.size() - 1] - 48;
			index = index < 1 || index > 3 ? 3 : index - 1;

			m_smFont.print(surface, x + 8, y + 20, day + m_ui->strings(index + 170));

			// draw month
			string month("JanFebMarAprMayJunJulAugSepOctNovDec");
			m_smFont.print(surface, x + 36, y + 20, month.substr(m_ui->state.time.getMonth() * 3, 3));

			// draw year
			string year(format("%d", m_ui->state.time.getYear()));
			textRect = Rect(x + 20, y + 27, m_smFont.getTextWidth(year), h);
			textRect.centerHorizontal(*this);

			m_smFont.print(surface, textRect.x, textRect.y, year);
		}
	};

	GeoScape::GeoScape(StartMode mode)
		: UIElement(0, 0, 320, 200), m_palette("geodata/palettes.dat"), m_mode(mode)
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

		create(new GeoScapeTimeButton(*m_globe, 257, 112, GameTime::Time5Sec));
		create(new GeoScapeTimeButton(*m_globe, 289, 112, GameTime::Time1Min));
		create(new GeoScapeTimeButton(*m_globe, 257, 126, GameTime::Time5Min));
		create(new GeoScapeTimeButton(*m_globe, 289, 126, GameTime::Time30Min));
		create(new GeoScapeTimeButton(*m_globe, 257, 140, GameTime::Time1Hour));
		create(new GeoScapeTimeButton(*m_globe, 289, 140, GameTime::Time1Day));

		create(new GeoScapeGlobeControl(*m_globe, 271, 162, 13, 13, GeoScapeGlobeControl::RotateUp));
		create(new GeoScapeGlobeControl(*m_globe, 271, 186, 13, 13, GeoScapeGlobeControl::RotateDown));
		create(new GeoScapeGlobeControl(*m_globe, 259, 174, 13, 13, GeoScapeGlobeControl::RotateLeft));
		create(new GeoScapeGlobeControl(*m_globe, 284, 174, 13, 13, GeoScapeGlobeControl::RotateRight));
		create(new GeoScapeGlobeControl(*m_globe, 295, 155, 23, 23, GeoScapeGlobeControl::ZoomIn, false));
		create(new GeoScapeGlobeControl(*m_globe, 299, 182, 15, 17, GeoScapeGlobeControl::ZoomOut, false));

		// create time display
		create(new GameTimeDisplay());

		// show base creation dialog if mode is set
		if (m_mode != Normal)
			create(new NewBaseDialog(m_mode != CreateFirstBase));
	}

	void GeoScape::draw(Surface& surface)
	{
		// draw background
		m_bg.blit(surface);
	}
}