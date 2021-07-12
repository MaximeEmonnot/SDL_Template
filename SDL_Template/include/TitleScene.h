#pragma once
#include "Scene.h"

#include "TextBox.h"
#include "BasicMenu.h"
#include "MiddleMenu.h"

class TitleScene : public Scene {

public:
	TitleScene(const std::string& backgroundSprite);
	~TitleScene();

	void Update(int& output) override;
	void Draw() override;
private:
	GraphicsEngine::Font font = GraphicsEngine::Font("ttf/arcadeClassic.TTF", 16);
	std::shared_ptr<CoreSystem::Mouse> pMouse;
	AMenu* menu;
	TextBox title;
};