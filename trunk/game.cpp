#include "game.h"
#include "worldmap.h"

namespace ufo
{
	Game::Game()
	{
		SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

		SDL_Surface* screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT);

		WorldMap map;

		map.radius = 5;
		map.distance = 2000;
		while (1)
		{
			SDL_FillRect(screen, NULL, 0);

			SDL_Event e;
			SDL_PollEvent(&e);
			if (e.type == SDL_QUIT)
				break;
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_UP)
					map.distance += 20;
				if (e.key.keysym.sym == SDLK_DOWN && map.distance > 20)
					map.distance -= 20;
				if (e.key.keysym.sym == SDLK_LEFT && map.radius > 0)
					map.radius--;
				if (e.key.keysym.sym == SDLK_RIGHT)
					map.radius++;
			}

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