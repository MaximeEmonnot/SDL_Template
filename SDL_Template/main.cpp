#include "Game.h"
int main(int argc, char* argv[])
{
	Window wnd;
	Game theGame(wnd);
	
	while (true) {
		theGame.Go();
	}

	return 0;
}