#pragma once
#include "Graphics.h"
#include "WindowManager.h"

class Game
{
public:
	Game(Window& wnd);
	~Game();

	void Go();

private:
	void UpdateFrame();
	void RenderFrame();
	// ***************** USER FUNCTIONS ******************//

	// ***************** END USER FUNCTIONS ************* //
private:
	Window& wnd;
	Graphics gfx;
	// ***************** USER VARIABLES ******************//
	SDL_Color color;
	SDL_Rect test;
	// ***************** END USER VARIABLES ************* //
};

