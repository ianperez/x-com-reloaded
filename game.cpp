#include "game.h"
#include "worldmap.h"

namespace ufo
{
	void init(SDL_Surface* surface, WorldMap& map)
	{
		map.radius = surface->h / 2 - 10;
		map.rx = 0;
		map.ry = 720;
	}

	Game::Game()
	{
		SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

		SDL_Surface* screen = SDL_SetVideoMode(320, 240, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT);

		WorldMap map;
		init(screen, map);
		map.test(screen);
		while (1)
		{
			SDL_FillRect(screen, NULL, 0);

			SDL_Event e;
			SDL_PollEvent(&e);
			if (e.type == SDL_QUIT)
				break;

			Uint8 *keystate = SDL_GetKeyState(NULL);
			if (keystate[SDLK_UP])
				map.ry -= 10;
			if (keystate[SDLK_DOWN])
				map.ry += 10;
			if (keystate[SDLK_LEFT])
				map.rx -= 10;
			if (keystate[SDLK_RIGHT])
				map.rx += 10;
			if (keystate[SDLK_KP0])
				init(screen, map);

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