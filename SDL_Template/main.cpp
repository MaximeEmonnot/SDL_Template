#include "Game.h"

int main(int argc, char* argv[])
{
	Window wnd;
	Game theGame(wnd);
	
	while (wnd.IsRunning()) {
		wnd.EventListener();
		theGame.Go();
	}

	return 0;
}