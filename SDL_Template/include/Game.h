#pragma once
#include "Graphics.h"
#include "Window.h"
#include <memory>
// ***** USER INCLUDES ***** //
#include "TitleScene.h"
#include "ExplorationScene.h"
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
	std::vector<std::shared_ptr<Scene>> scenes;
	std::vector<std::shared_ptr<Scene>>::iterator currentScene;
	// ***************** END USER VARIABLES ************* //
};

