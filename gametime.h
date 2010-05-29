#pragma once
#include <sdl.h>
#include <string>
#include "surface.h"
#include "font.h"

namespace ufo
{
	using namespace std;

	class GameTime
	{
		Uint32 getMonthDays(Uint32 month);

	public:

		GameTime();

		enum Step
		{
			Time5Sec,
			Time1Min,
			Time5Min,
			Time30Min,
			Time1Hour,
			Time1Day
		};

		void setStep(Step step);
		void increment(Uint32 timeElapsed);

		Uint32 getYear() const { return m_year; }
		Uint32 getMonth() const { return m_month; }
		Uint32 getWeekDay() const { return m_weekday; }
		Uint32 getDay() const { return m_day; }
		Uint32 getHour() const { return m_hour; }
		Uint32 getMinute() const { return m_minute; }
		Uint32 getSecond() const { return m_second; }

	protected:

		Uint32 m_year;
		Uint32 m_month;
		Uint32 m_weekday;
		Uint32 m_day;
		Uint32 m_hour;
		Uint32 m_minute;
		Uint32 m_second;

		Step m_step;
		Uint32 m_speed;

		Uint32 m_timeElapsed;
	};
}