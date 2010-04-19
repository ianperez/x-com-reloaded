#include "game.h"
#include "worldmap.h"

namespace ufo
{
	void init(WorldMap& map)
	{
		map.radius = 1;
		map.distance = 3000000;
		map.rx = 0;
		map.ry = 0;
		map.rz = 0;
		map.lx = 0;
		map.ly = 720;
	}

	Game::Game()
	{
		SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

		SDL_Surface* screen = SDL_SetVideoMode(320, 240, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT);

		WorldMap map;
		init(map);
		map.test(screen);
		map.rx = 0;
		map.ry = 0;
		while (1)
		{
			SDL_FillRect(screen, NULL, 0);

			SDL_Event e;
			SDL_PollEvent(&e);
			if (e.type == SDL_QUIT)
				break;

			Uint8 *keystate = SDL_GetKeyState(NULL);
			if (keystate[SDLK_UP])
				map.ry -= 0.01;
			if (keystate[SDLK_DOWN])
				map.ry += 0.01;
			if (keystate[SDLK_LEFT])
				map.rx -= 0.01;
			if (keystate[SDLK_RIGHT])
				map.rx += 0.01;
			if (keystate[SDLK_HOME])
				map.ly -= 5;
			if (keystate[SDLK_END])
				map.ly += 5;
			if (keystate[SDLK_DELETE])
				map.lx -= 5;
			if (keystate[SDLK_PAGEDOWN])
				map.lx += 5;
			if (keystate[SDLK_PAGEUP])
				map.distance += 10000;
			if (keystate[SDLK_INSERT])
				map.distance -= 10000;
			if (keystate[SDLK_KP0])
				init(map);

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