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
	void ComputeCorsairColors();

private:
	void UpdateFrame();
	void RenderFrame();
	// ***************** USER FUNCTIONS ******************//

	// ***************** END USER FUNCTIONS ************* //
private:
	Window& wnd;
	Graphics gfx;
	// ***************** USER VARIABLES ******************//
	int kbdWidth;
	int kbdHeight;
	SDL_Rect squarePlayer;
	SDL_Rect squareEnemy;
	float impulsionForce = 10.0f;
	bool lost = false;
	// ***************** END USER VARIABLES ************* //
};

