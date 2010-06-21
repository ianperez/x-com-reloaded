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
		string getSavePath(Uint16 slot);

	public:

		GameState();

		void save(Uint16 slot);
		void load(Uint16 slot);

		GameTime time;
		GlobeState globe;
		BaseState bases;

		Uint32 funds;
	};
}