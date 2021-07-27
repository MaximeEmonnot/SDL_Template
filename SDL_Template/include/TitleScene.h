#pragma once
#include "Scene.h"
#include "TitleMenu.h"

class TitleScene : public Scene {

public:
	TitleScene();

	void Update() override;
	void Draw() override;
	
private:
	std::shared_ptr<CoreSystem::Window> pWnd;
	std::shared_ptr<CoreSystem::Mouse> pMouse;
	std::shared_ptr<Player> pPlayer;
	std::unique_ptr<AMenu> titleMenu;
	TextBox title;
};