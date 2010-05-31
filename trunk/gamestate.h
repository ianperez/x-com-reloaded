#pragma once
#include "gametime.h"

namespace ufo
{
	class GameState
	{
		string getSavePath(Uint16 slot);

	public:

		void save(Uint16 slot);
		void load(Uint16 slot);

		GameTime time;
	};
}