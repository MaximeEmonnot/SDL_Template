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
	void CalculateFrame();
	void RenderFrame();

	//New functions
private:
	Window& wnd;
	Graphics gfx;

};

