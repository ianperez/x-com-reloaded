#include "baseview.h"
#include "uimanager.h"
#include "uibutton.h"
#include "geoscape.h"
#include "globe.h"

namespace ufo
{
	class BaseViewButton : public UIPushButtonStandard
	{
	public:

		BaseViewButton(Uint16 stringId, Uint8 index)
			: UIPushButtonStandard(stringId, 192, 58 + index * 13, 128, 12, Palette::blockSize * 13 + 6) { }

		void onPress()
		{
			if (m_stringId == 57)
			{
				m_ui->destroy(m_parent);
				m_ui->create(new GeoScape(Globe::CreateBase));
			}
			else if (m_stringId == 66)
			{
				m_ui->destroy(m_parent);
				m_ui->create(new GeoScape());
			}
		}
	};

	BaseView::BaseView()
		: UIElement(0, 0, 320, 200)
	{
	}

	void BaseView::onCreate()
	{
		Palette p("geodata/palettes.dat", 1);
		p.apply(m_ui->surface);

		create(new BaseViewButton(57, 0));
		create(new BaseViewButton(58, 1));
		create(new BaseViewButton(820, 2));
		create(new BaseViewButton(59, 3));
		create(new BaseViewButton(60, 4));
		create(new BaseViewButton(61, 5));
		create(new BaseViewButton(62, 6));
		create(new BaseViewButton(63, 7));
		create(new BaseViewButton(64, 8));
		create(new BaseViewButton(65, 9));
		create(new BaseViewButton(66, 10));
	}

	void BaseView::draw(Surface& surface)
	{
	}
}