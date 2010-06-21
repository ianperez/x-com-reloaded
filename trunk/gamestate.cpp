#include <sstream>
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

	string GameState::getSavePath(Uint16 slot)
	{
		stringstream path;
		path << "game_" << slot << '/';
		return path.str();
	}

	void GameState::save(Uint16 slot)
	{
		string path(getSavePath(slot));
		for (size_t i = 0; i < size(); ++i)
			at(i)->save(path);
	}

	void GameState::load(Uint16 slot)
	{
		string path(getSavePath(slot));
		for (size_t i = 0; i < size(); ++i)
			at(i)->load(path);
	}
}