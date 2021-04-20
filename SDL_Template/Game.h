#pragma once
#include "Graphics.h"
#include "WindowManager.h"
#include "Surface.h"
#include "Animation.h"
#include "Timer.h"

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
	Surface test;
	SDL_Rect testSurface;
	Surface megaman;
	Animation megamanAnimation;
	Timer t;
	// ***************** END USER VARIABLES ************* //
};

