#include "gamestate.h"
#include "util.h"

namespace ufo
{
	GameState::GameState()
		: funds(4147000)
	{
		push_back(&time);
		push_back(&globe);
		push_back(&bases);
	}

	void GameState::save(Uint8 slot)
	{
		string path(format("game_%d/", slot));
		for (size_t i = 0; i < size(); ++i)
			at(i)->save(path);
	}

	void GameState::load(Uint8 slot)
	{
		string path(format("game_%d/", slot));
		for (size_t i = 0; i < size(); ++i)
			at(i)->load(path);
	}
}