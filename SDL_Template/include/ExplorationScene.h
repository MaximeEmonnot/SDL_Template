#pragma once
#include "Scene.h"
#include "Grid.h"
#include "Font.h"

class ExplorationScene : public Scene
{
public:
	ExplorationScene(const std::string& backgroundPath);
	~ExplorationScene();

	void Update() override;
	void Draw() override;

private:
	GraphicsEngine::Font font = GraphicsEngine::Font("ttf/arcadeClassic.TTF", 32);

	std::shared_ptr<Grid> pGrid;
	std::shared_ptr<Player> pPlayer;
	std::shared_ptr<CoreSystem::Timer> pTimer;
	std::shared_ptr<CoreSystem::Keyboard> pKbd;
};