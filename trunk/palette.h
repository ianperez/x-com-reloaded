#pragma once
#include <sdl.h>
#include <string>

namespace ufo
{
	using namespace std;
	class Palette
	{
		void load(string filename);

	public:

		Palette();
		Palette(string filename);
	};
}