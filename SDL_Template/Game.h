#pragma once
#include "Graphics.h"
#include "WindowManager.h"
#include "Timer.h"
#include "Character.h"
#include "Font.h"

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
	Character kirby;
	SDL_Rect destKirby;
	Timer t;
	Font font;
	// ***************** END USER VARIABLES ************* //
};

