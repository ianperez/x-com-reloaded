#pragma once
#include <string>
#include <vector>

namespace ufo
{
	using namespace std;

	class StringTable : public vector<vector<string> >
	{
	public:

		enum Language
		{
			English,
			German,
			French
		};

		Language getLanguage() const { return m_language; }

		void load(string filename);

		string& operator() (size_t index);

	private:

		Language m_language;
	};
}