#pragma once
#include "Graphics.h"
#include "Window.h"
#include <memory>
// ***** USER INCLUDES ***** //
#include "Circle.h"
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
	std::vector<FCircle> circles;
	static constexpr SDL_Color colors[9] = { RED, GREEN, BLUE, MAGENTA, CYAN, YELLOW, WHITE, GRAY, LIGHTGRAY};
	int currentIndex = 0;
	// ***************** END USER VARIABLES ************* //
};

