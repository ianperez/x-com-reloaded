#include "game.h"
#include "dialog.main.h"
#include "uimanager.h"
#include <sdl_rotozoom.h>

namespace ufo
{
	Game::Game()
	{
		SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
		SDL_WM_SetCaption("x-com-reloaded", "");

		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

		Surface screen(SDL_SetVideoMode(320, 200, 8, SDL_HWSURFACE | SDL_HWPALETTE));

		StringTable strings;

		UIManager ui(screen, strings);
		ui.create(new LanguageDialog());

		while (1)
		{
			screen.fillRect(NULL, 0);

			SDL_Event e;
			if (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
					break;

				ui.processEvent(e);
			}

			ui.draw();

			screen.flip();
		}
	}

	Game::~Game()
	{
		SDL_Quit();
	}
}