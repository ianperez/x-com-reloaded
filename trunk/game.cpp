#include "game.h"
#include "worldmap.h"
#include "uimanager.h"
#include <sdl_rotozoom.h>
#include <boost/shared_ptr.hpp>

using namespace boost;

namespace ufo
{
	Game::Game()
	{
		SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
		SDL_WM_SetCaption("x-com-reloaded", "");

		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

		SDL_Surface* screen = SDL_SetVideoMode(640, 400, 32, SDL_HWSURFACE);
		SDL_Surface* main = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 200, 32, 0, 0, 0, 0);

		bool xscale = screen->w * 1.0 / main->w < screen->h * 1.0 / main->h;
		double scale = xscale ? screen->w * 1.0 / main->w : screen->h* 1.0 / main->h;

		SDL_Rect dst;
		dst.x = xscale ? 0 : static_cast<Sint16>((screen->w - main->w * scale) / 2);
		dst.y = xscale ? static_cast<Sint16>((screen->h - main->h * scale) / 2) : 0;

		UIManager ui;

		shared_ptr<WorldMap> map(new WorldMap);
		ui.add(map);

		while (1)
		{
			SDL_FillRect(main, NULL, 0);

			SDL_Event e;
			if (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
					break;

				// scale mouse events
				if (e.type == SDL_MOUSEBUTTONUP || e.type == SDL_MOUSEBUTTONDOWN)
				{
					e.button.x = static_cast<Sint16>(e.button.x / scale - dst.x / scale);
					e.button.y = static_cast<Sint16>(e.button.y / scale - dst.y / scale);
				}
				else if (e.type == SDL_MOUSEMOTION)
				{
					e.motion.x = static_cast<Sint16>(e.motion.x / scale - dst.x / scale);
					e.motion.y = static_cast<Sint16>(e.motion.y / scale - dst.y / scale);
				}

				ui.processEvent(e);
			}

			ui.draw(main);

			SDL_Surface* temp = zoomSurface(main, scale, scale, 1);
			SDL_BlitSurface(temp, NULL, screen, &dst);
			SDL_FreeSurface(temp);

			SDL_Flip(screen);
		}
	}

	Game::~Game()
	{
		SDL_Quit();
	}
}