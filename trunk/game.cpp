#include "game.h"
#include "dialog.main.h"
#include "uimanager.h"
#include "util.h"
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

		Surface screen(SDL_SetVideoMode(640, 400, 32, SDL_HWSURFACE));
		Surface main(SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 200, 8, 0, 0, 0, 0));

		bool xscale = screen.w * 1.0 / main.w < screen.h * 1.0 / main.h;
		double scale = xscale ? screen.w * 1.0 / main.w : screen.h* 1.0 / main.h;

		Rect dst;
		dst.x = xscale ? 0 : round<Sint16>((screen.w - main.w * scale) / 2);
		dst.y = xscale ? round<Sint16>((screen.h - main.h * scale) / 2) : 0;

		StringTable strings;
		UIManager ui(main, strings);
		ui.create(new LanguageDialog());

		while (1)
		{
			screen.fillRect(NULL, 0);

			SDL_Event e;
			if (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
					break;

				// scale mouse events
				if (e.type == SDL_MOUSEBUTTONUP || e.type == SDL_MOUSEBUTTONDOWN)
				{
					e.button.x = round<Sint16>(e.button.x / scale - dst.x / scale);
					e.button.y = round<Sint16>(e.button.y / scale - dst.y / scale);
				}
				else if (e.type == SDL_MOUSEMOTION)
				{
					e.motion.x = round<Sint16>(e.motion.x / scale - dst.x / scale);
					e.motion.y = round<Sint16>(e.motion.y / scale - dst.y / scale);
				}

				ui.processEvent(e);
			}

			ui.draw();

			Surface temp(zoomSurface(main.get(), scale, scale, 1));
			temp.blit(screen, &dst);

			screen.flip();
		}
	}

	Game::~Game()
	{
		SDL_Quit();
	}
}