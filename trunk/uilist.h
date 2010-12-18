#pragma once

#include <sdl.h>
#include <vector>
#include <string>
#include "uielement.h"
#include "point.h"

namespace ufo
{
	using namespace std;

	class Column
	{
		Uint16 m_titleStringId;
		Uint16 m_width;
		Uint8 m_titleColor;
		bool m_fill;
		Point::Screen m_headerOffset;

	public:

		Column() { }
		Column(Uint16 titleStringId, Uint16 width, Uint8 titleColor, bool fill = false);
		Column(Uint16 titleStringId, Uint16 width, Uint8 titleColor, bool fill, Point::Screen headerOffset);

		Uint16 getTitleStringId() const { return m_titleStringId; }
		Uint16 getWidth() const { return m_width; }
		Uint8 getTitleColor() const { return m_titleColor; }
		bool fill() const { return m_fill; }
		Point::Screen getHeaderOffset() const { return m_headerOffset; }
	};

	class UIList : public UIElement
	{
	public:

		virtual bool getCell(Uint16 row, Uint16 col, string& data, Uint8& color) { return false; }

		void draw(Surface& surface);

	protected:

		vector<Column> m_columns;
	};
}