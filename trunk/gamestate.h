#pragma once
#include <vector>
#include <boost/shared_ptr.hpp>
#include "gametime.h"
#include "base.h"

namespace ufo
{
	using namespace std;
	using namespace boost;

	class GameState
	{
		string getSavePath(Uint16 slot);

	public:

		void save(Uint16 slot);
		void load(Uint16 slot);

		GameTime time;
		vector<shared_ptr<Base> > bases;

		Uint32 funds;
	};
}