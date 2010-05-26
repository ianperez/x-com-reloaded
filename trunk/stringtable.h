#pragma once
#include <string>
#include <vector>

namespace ufo
{
	using namespace std;

	class StringTable : public vector<vector<string> >
	{
	public:

		void load(string filename);
	};
}