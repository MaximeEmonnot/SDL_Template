#pragma once
#include "Scene.h"

#include "TextBox.h"
#include "BasicMenu.h"
#include "MiddleMenu.h"

class TitleScene : public Scene {

public:
	TitleScene();

	void Update() override;
	void Draw() override;
	
private:
	std::shared_ptr<CoreSystem::Window> pWnd;
	std::shared_ptr<CoreSystem::Mouse> pMouse;
	std::shared_ptr<Player> pPlayer;
	std::unique_ptr<AMenu> menu;
	TextBox title;
};