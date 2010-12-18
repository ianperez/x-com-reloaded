#pragma once

#include <string>
#include <sdl.h>

namespace ufo
{
	using namespace std;

	class SaveInfo
	{
	public:

		enum SaveType
		{
			Geoscape,
			Tactical
		};

		SaveInfo() { }
		SaveInfo(Uint8 slot);

		void save(Uint8 slot);
		void load(Uint8 slot);

		string getName() const { return m_name; }
		Uint16 getYear() const { return m_year; }
		Uint16 getMonth() const { return m_month; }
		Uint16 getDay() const { return m_day; }
		Uint16 getHour() const { return m_hour; }
		Uint16 getMinute() const { return m_minute; }
		SaveType getType() const { return m_type; }

	private:

		string m_name;
		Uint16 m_year;
		Uint16 m_month;
		Uint16 m_day;
		Uint16 m_hour;
		Uint16 m_minute;
		SaveType m_type;
	};
}