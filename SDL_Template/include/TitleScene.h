#pragma once
#include "Scene.h"
#include "TitleMenu.h"

class TitleScene : public Scene {

public:
	TitleScene(std::shared_ptr<Player> pPlayer);

	void Update() override;
	void Draw() override;
	
private:
	std::unique_ptr<AMenu> titleMenu;
	TextBox title;
};