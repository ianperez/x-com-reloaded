#pragma once
#include <sdl.h>

namespace ufo
{
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

	protected:

		Uint32 m_year;
		Uint32 m_month;
		Uint32 m_weekday;
		Uint32 m_day;
		Uint32 m_hour;
		Uint32 m_minute;
		Uint32 m_second;

		Step m_step;
		double m_speed;
	};
}