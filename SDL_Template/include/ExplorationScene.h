#pragma once
#include "Scene.h"

class ExplorationScene : public Scene
{
public:
	ExplorationScene(const std::string& backgroundPath);

	void Update(int& output) override;
	void Draw() override;

private:
	Player player;
	std::shared_ptr<CoreSystem::Timer> pTimer;
};