#pragma once
#include <string>
#include <vector>

namespace ufo
{
	using namespace std;

	class StringTable : public vector<vector<string> >
	{
	public:

		StringTable();

		enum Language
		{
			English,
			German,
			French
		};

		Language getLanguage() const { return m_language; }
		void setLanguage(Language language) { m_language = language; }

		void load(string filename);

		string& operator() (size_t index);

	private:

		Language m_language;
	};
}