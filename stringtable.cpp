#include "stringtable.h"
#include <fstream>

namespace ufo
{
	void StringTable::load(string filename)
	{
		ifstream infile(filename.c_str());
		if (!infile)
			throw runtime_error("error opening " + filename);

		char c;
		string buffer;
		vector<string> temp;
		while (!infile.get(c).eof())
		{
			if (!c)
			{
				temp.push_back(buffer);
				push_back(temp);
				temp.clear();
				buffer.clear();
			}
			else if (c == 2)
			{
				temp.push_back(buffer);
				buffer.clear();
			}
			else
				buffer += c;
		}
	}

	string& StringTable::operator() (size_t index)
	{
		return at(index).at(0);
	}
}