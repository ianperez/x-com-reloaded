#include "game.h"
#include "worldmap.h"

namespace ufo
{
	Game::Game()
	{
		SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

		SDL_Surface* screen = SDL_SetVideoMode(320, 240, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT);

		WorldMap map;

		map.distance = 10000;
		while (1)
		{
			SDL_FillRect(screen, NULL, 0);

			SDL_Event e;
			SDL_PollEvent(&e);
			if (e.type == SDL_QUIT)
				break;

			Uint8 *keystate = SDL_GetKeyState(NULL);
			if (keystate[SDLK_UP])
				map.distance += 200;
			if (keystate[SDLK_DOWN] && map.distance > 200)
				map.distance -= 200;

			map.draw(screen);

			SDL_Flip(screen);
			SDL_Delay(10);
		}
	}

	Game::~Game()
	{
		SDL_Quit();
	}
}