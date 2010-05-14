#include "game.h"
#include "worldmap.h"
#include "uimanager.h"
#include <boost/shared_ptr.hpp>

using namespace boost;

namespace ufo
{
	Game::Game()
	{
		SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
		SDL_WM_SetCaption("x-com-reloaded", "");

		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

		SDL_Surface* screen = SDL_SetVideoMode(320, 200, 24, SDL_HWSURFACE | SDL_DOUBLEBUF);

		UIManager ui;

		shared_ptr<WorldMap> map(new WorldMap);
		ui.add(map);

		while (1)
		{
			SDL_FillRect(screen, NULL, 0);

			SDL_Event e;
			if (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
					break;
				ui.processEvent(e);
			}

			ui.draw(screen);

			SDL_Flip(screen);
			SDL_Delay(10);
		}
	}

	Game::~Game()
	{
		SDL_Quit();
	}
}