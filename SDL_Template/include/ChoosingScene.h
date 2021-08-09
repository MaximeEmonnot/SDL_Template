#pragma once
#include "Scene.h"
#include "Player.h"

#include "ChoiceMenu.h"
#include "Graphics.h"

class ChoosingScene : public Scene {
public:
	ChoosingScene();

	void Update() override;
	void Draw() override;

private:
	Pokemon& GetChoice(int choice);

private:
	Pokemon choice0;
	Pokemon choice1;
	Pokemon choice2;
	std::unique_ptr<AMenu> choiceMenu;
};