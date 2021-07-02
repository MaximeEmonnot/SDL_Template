#pragma once
#include "Graphics.h"
#include "Window.h"
#include <memory>
// ***** USER INCLUDES ***** //
#include "TextBox.h"
#include "Player.h"
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

	TextBox box;

	Player player;

	GraphicsEngine::Font font = GraphicsEngine::Font("ttf/arcadeClassic.TTF", 16);
	// ***************** END USER VARIABLES ************* //
};

