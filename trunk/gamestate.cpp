#include <sstream>
#include "gamestate.h"
#include "util.h"

namespace ufo
{
	GameState::GameState()
		: funds(4147000)
	{
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
		time.save(path);
	}

	void GameState::load(Uint16 slot)
	{
		string path(getSavePath(slot));
		time.load(path);
		globe.load(path);
	}
}