#pragma once
#include "Scene.h"

#include "Player.h"

#include "BasicMenu.h"
#include "BottomMenu.h"
#include "Graphics.h"

class ChoosingScene : public Scene {
public:
	ChoosingScene(const std::string& backgroundSprite);
	~ChoosingScene();

	void Update() override;
	void Draw() override;

private:
	Pokemon& GetChoice(int choice);

private:
	GraphicsEngine::Font font = GraphicsEngine::Font("ttf/arcadeClassic.TTF", 16);

	std::shared_ptr<Player> pPlayer;
	std::shared_ptr <CoreSystem::Mouse> pMouse; 
	Pokemon choice0;
	Pokemon choice1;
	Pokemon choice2;
	std::unique_ptr<AMenu> choiceMenu;
};