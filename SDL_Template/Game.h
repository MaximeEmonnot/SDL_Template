#pragma once
#include "Graphics.h"
#include "Window.h"
#include <memory>
// ***** USER INCLUDES ***** //
#include "Character.h"
// *** END USER INCLUDES *** //


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
	// ***************** END USER VARIABLES ************* //
};

