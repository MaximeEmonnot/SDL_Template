#pragma once
#include "Graphics.h"
#include "Window.h"

#include "Character.h"
#include <memory>

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
	std::unique_ptr<Character> kirby;
	SDL_Rect destKirby;
	// ***************** END USER VARIABLES ************* //
};

