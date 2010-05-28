#include "gametime.h"
#include "util.h"

namespace ufo
{
	GameTime::GameTime()
		: m_month(0), m_weekday(5), m_day(0), m_hour(12), m_minute(0), m_second(0)
	{
	}

	Uint32 GameTime::getMonthDays(Uint32 month)
	{
		bool leap = false;
		if ((m_year % 400) == 0)
			leap = true;
		else if ((m_year % 100) == 0)
			leap = false;
		else if ((m_year % 4) == 0)
			leap = true;

		Uint8 days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		days[1] = leap ? 29 : 28;
		return days[month];
	}

	void GameTime::setStep(Step step)
	{
		m_step = step;
	}

	void GameTime::increment(Uint32 timeElapsed)
	{
		Uint32 delta = round<Uint32>(timeElapsed / m_speed) * 5;
		m_second += delta;
		m_minute += m_second / 60;
		m_hour += m_minute / 60;
		m_day += m_hour / 24;
		m_weekday += m_hour / 24;
		m_month += m_day / getMonthDays(m_month);
	}
}