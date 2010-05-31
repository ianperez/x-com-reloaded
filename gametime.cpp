#include <fstream>
#include "gametime.h"
#include "util.h"

namespace ufo
{
	GameTime::GameTime()
		: m_year(1999), m_month(0), m_weekday(5), m_day(0), m_hour(12), m_minute(0), m_second(0), m_speed(200), m_step(Time5Sec), m_timeElapsed(0), m_pause(false)
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
		if (leap)
			++days[1];

		return days[month];
	}

	void GameTime::setStep(Step step)
	{
		m_step = step;
	}

	void GameTime::increment(Uint32 timeElapsed)
	{
		m_timeElapsed += timeElapsed;
		if (m_timeElapsed / m_speed > 0)
		{
			if (!m_pause)
			{
				Uint32 steps[] = { 5, 60, 300, 1800, 3600, 86400 };
				Uint32 monthDays = getMonthDays(m_month);

				Uint32 delta = m_timeElapsed / m_speed * steps[m_step];
				m_second += delta;
				m_minute += m_second / 60;
				m_hour += m_minute / 60;
				m_day += m_hour / 24;
				m_weekday += m_hour / 24;
				m_month += m_day / monthDays;
				m_year += m_month / 12;

				m_second %= 60;
				m_minute %= 60;
				m_hour %= 24;
				m_day %= monthDays;
				m_weekday %= 7;
				m_month %= 12;
			}

			m_timeElapsed = 0;
		}
	}

	void GameTime::save(string path)
	{
		string filename(path + "iglob.dat");
		ofstream file(filename.c_str());
		if (!file)
			throw runtime_error("error opening " + filename);

		file.write((char*)&m_month, 4);
		file.write((char*)&m_weekday, 4);
		file.write((char*)&m_day, 4);
		file.write((char*)&m_hour, 4);
		file.write((char*)&m_minute, 4);
		file.write((char*)&m_second, 4);

		file.close();

		filename = path + "uiglob.dat";
		file.open(filename.c_str());
		if (!file)
			throw runtime_error("error opening " + filename);

		file.seekp(22, ios::beg);

		file.write((char*)&m_year, 2);
	}

	void GameTime::load(string path)
	{
		string filename(path + "iglob.dat");
		ifstream file(filename.c_str());
		if (!file)
			throw runtime_error("error opening " + filename);

		file.read((char*)&m_month, 4);
		file.read((char*)&m_weekday, 4);
		file.read((char*)&m_day, 4);
		file.read((char*)&m_hour, 4);
		file.read((char*)&m_minute, 4);
		file.read((char*)&m_second, 4);

		file.close();

		filename = path + "uiglob.dat";
		file.open(filename.c_str());
		if (!file)
			throw runtime_error("error opening " + filename);

		file.seekg(22, ios::beg);

		file.read((char*)&m_year, 2);
	}
}