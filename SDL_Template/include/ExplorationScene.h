#pragma once
#include "Scene.h"

class ExplorationScene : public Scene
{
public:
	ExplorationScene(const std::string& backgroundPath);

	void Update() override;
	void Draw() override;

private:
	std::shared_ptr<Player> pPlayer;
	std::shared_ptr<CoreSystem::Timer> pTimer;
	std::shared_ptr<CoreSystem::Keyboard> pKbd;
};