#include "saveinfo.h"
#include <fstream>
#include "util.h"

namespace ufo
{
	SaveInfo::SaveInfo(Uint8 slot)
	{
		load(slot);
	}

	void SaveInfo::load(Uint8 slot)
	{
		string filename(format("game_%d/saveinfo.dat", slot));
		ifstream file(filename.c_str(), ios::binary);
		if (!file)
			throw runtime_error("error opening " + filename);

		file.seekg(2, ios::beg);

		char name[26];
		file.read(name, 26);
		m_name = name;

		file.read((char*)&m_year, 2);
		file.read((char*)&m_month, 2);
		file.read((char*)&m_day, 2);
		file.read((char*)&m_hour, 2);
		file.read((char*)&m_minute, 2);

		Uint16 type;
		file.read((char*)&type, 2);
		m_type = type == 0 ? Geoscape : Tactical;
	}
}