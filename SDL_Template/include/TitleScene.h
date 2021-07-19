#pragma once
#include "Scene.h"

#include "TextBox.h"
#include "BasicMenu.h"
#include "MiddleMenu.h"

class TitleScene : public Scene {

public:
	TitleScene(const std::string& backgroundSprite);
	~TitleScene();

	void Update() override;
	void Draw() override;
	
private:
	GraphicsEngine::Font font = GraphicsEngine::Font("ttf/arcadeClassic.TTF", 16);
	std::shared_ptr<CoreSystem::Window> pWnd;
	std::shared_ptr<CoreSystem::Mouse> pMouse;
	std::shared_ptr<Player> pPlayer;
	AMenu* menu;
	TextBox title;
};