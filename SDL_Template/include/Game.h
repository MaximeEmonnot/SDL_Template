#pragma once
#include "Graphics.h"
#include "Timer.h"
#include <memory>
// ***** USER INCLUDES ***** //
#include "TitleScene.h"
#include "ChoosingScene.h"
#include "ExplorationScene.h"
#include "FightingScene.h"
// *** END USER INCLUDES *** //


class Game
{
public:
	Game();

	void Go();
	void ComputeCorsairColors();

private:
	void UpdateFrame();
	void RenderFrame();
	// ***************** USER FUNCTIONS ******************//

	// ***************** END USER FUNCTIONS ************* //
private:
	std::shared_ptr<Player> pPlayer;
	// ***************** USER VARIABLES ******************//
	std::vector<std::shared_ptr<Scene>>				scenes;
	std::vector<std::shared_ptr<Scene>>::iterator	currentScene;
	// ***************** END USER VARIABLES ************* //
};

