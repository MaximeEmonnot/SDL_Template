#pragma once
#include "Graphics.h"
#include "Window.h"
#include <memory>
// ***** USER INCLUDES ***** //

// *** END USER INCLUDES *** //


class Game
{
public:
	Game(CoreSystem::Window& wnd);
	~Game();

	void Go();
	void ComputeCorsairColors();

private:
	void UpdateFrame();
	void RenderFrame();
	// ***************** USER FUNCTIONS ******************//

	// ***************** END USER FUNCTIONS ************* //
private:
	CoreSystem::Window& wnd;
	GraphicsEngine::Graphics gfx;
	// ***************** USER VARIABLES ******************//

	// ***************** END USER VARIABLES ************* //
};

