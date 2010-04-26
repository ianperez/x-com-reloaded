#include "game.h"
#include "worldmap.h"

namespace ufo
{
	Game::Game()
	{
		SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

		SDL_Surface* screen = SDL_SetVideoMode(320, 240, 32, SDL_HWSURFACE | SDL_HWPALETTE);

		WorldMap map(screen);
		while (1)
		{
			SDL_FillRect(screen, NULL, 0);

			SDL_Event e;
			SDL_PollEvent(&e);
			if (e.type == SDL_QUIT)
				break;
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				map.onClick(e.button.x, e.button.y);
			}

			Uint8 *keystate = SDL_GetKeyState(NULL);
			if (keystate[SDLK_UP] && map.ry > -720)
				map.ry -= 8;
			if (keystate[SDLK_DOWN] && map.ry < 720)
				map.ry += 8;
			if (keystate[SDLK_LEFT])
				map.rx -= 8;
			if (keystate[SDLK_RIGHT])
				map.rx += 8;
			if (keystate[SDLK_KP0])
				map.rx = map.ry = 0;

			map.draw();

			SDL_Flip(screen);
			SDL_Delay(10);
		}
	}

	Game::~Game()
	{
		SDL_Quit();
	}
}