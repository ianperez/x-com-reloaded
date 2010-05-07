#include "game.h"
#include "worldmap.h"
#include "uimanager.h"

namespace ufo
{
	Game::Game()
	{
		SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

		SDL_Surface* screen = SDL_SetVideoMode(320, 240, 24, SDL_HWSURFACE | SDL_DOUBLEBUF);

		WorldMap map(screen);
		while (1)
		{
			SDL_FillRect(screen, NULL, 0);

			SDL_Event e;
			if (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
					break;
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					if (e.button.button == SDL_BUTTON_LEFT)
						map.onClick(e.button.x, e.button.y);
					if (e.button.button == SDL_BUTTON_RIGHT)
						map.setDefaultTarget(e.button.x, e.button.y);
				}
			}

			Uint8 *keystate = SDL_GetKeyState(NULL);
			if (keystate[SDLK_UP])
				map.rotateVert(-8);
			if (keystate[SDLK_DOWN])
				map.rotateVert(8);
			if (keystate[SDLK_LEFT])
				map.rotateHorz(-8);
			if (keystate[SDLK_RIGHT])
				map.rotateHorz(8);
			if (keystate[SDLK_PAGEUP])
				map.zoom(10);
			if (keystate[SDLK_PAGEDOWN])
				map.zoom(-10);

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