#pragma once
#include "Scene.h"

#include "Player.h"

#include "BasicMenu.h"
#include "BottomMenu.h"
#include "Graphics.h"

class ChoosingScene : public Scene {
public:
	ChoosingScene();
	~ChoosingScene();

	void Update() override;
	void Draw() override;

private:
	Pokemon& GetChoice(int choice);

private:
	std::shared_ptr<Player> pPlayer;
	std::shared_ptr <CoreSystem::Mouse> pMouse; 
	Pokemon choice0;
	Pokemon choice1;
	Pokemon choice2;
	std::unique_ptr<AMenu> choiceMenu;
};