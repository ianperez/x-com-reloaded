#include "uilist.h"
#include "uimanager.h"

namespace ufo
{
	Column::Column(Uint16 titleStringId, Uint16 width, Uint8 titleColor, bool fill)
		: m_titleStringId(titleStringId), m_width(width), m_titleColor(titleColor), m_fill(fill)
	{
	}

	Column::Column(Uint16 titleStringId, Uint16 width, Uint8 titleColor, bool fill, Point::Screen headerOffset)
		: m_titleStringId(titleStringId), m_width(width), m_titleColor(titleColor), m_fill(fill), m_headerOffset(headerOffset)
	{
	}

	void UIList::draw(Surface& surface)
	{
		Sint16 _x = x;
		for (size_t i = 0; i < m_columns.size(); ++i)
		{
			m_ui->text.setColor(m_columns[i].getTitleColor());
			m_ui->text.print(surface, Rect(_x, y, m_columns[i].getWidth(), 0), m_columns[i].getTitleStringId(), TextRenderer::SmallFont);

			string data;
			Uint8 color;
			for (Uint16 j = 0; getCell(j, i, data, color); ++j)
			{
				m_ui->text.setColor(color);
				m_ui->text.print(surface, Rect(
			}

			_x += m_columns[i].getWidth();
		}
	}
}