#pragma once
#include "Graphics.h"
#include "Window.h"
#include <memory>
// ***** USER INCLUDES ***** //

// *** END USER INCLUDES *** //


class Game
{
public:
	Game();
	~Game();

	void Go();
	void ComputeCorsairColors();

private:
	void UpdateFrame();
	void RenderFrame();
	// ***************** USER FUNCTIONS ******************//

	// ***************** END USER FUNCTIONS ************* //
private:
	std::shared_ptr<CoreSystem::Window> mpWnd;
	std::shared_ptr<GraphicsEngine::Graphics> mpGfx;
	// ***************** USER VARIABLES ******************//

	// ***************** END USER VARIABLES ************* //
};

