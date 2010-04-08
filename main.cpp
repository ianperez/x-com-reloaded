#include "game.h"
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace std;
using namespace ufo;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	try
	{
		Game game;
	}
	catch (exception& e)
	{
		MessageBox(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
	}

	return 0;
}