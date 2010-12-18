#pragma once
#include <vector>
#include "gametime.h"
#include "base.h"
#include "globe.h"
#include "objectstate.h"

namespace ufo
{
	using namespace std;
	using namespace boost;

	class GameState : public vector<ObjectState*>
	{
	public:

		GameState();

		void save(Uint8 slot);
		void load(Uint8 slot);

		GameTime time;
		GlobeState globe;
		BaseState bases;

		Uint32 funds;
	};
}