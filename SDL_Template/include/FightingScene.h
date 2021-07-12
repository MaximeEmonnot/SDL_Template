#pragma once
#include "Scene.h"
#include "Player.h"

#include "BasicMenu.h"
#include "RightMenu.h"

class FightingScene : public Scene {
public:
	FightingScene(const std::string& backgroundSprite);
	~FightingScene();

	void Update() override;
	void Draw() override;

private:
	Pokemon* CreateRandomPokemon();

	void Fight();
	void Flee();

private:
	GraphicsEngine::Font font = GraphicsEngine::Font("ttf/arcadeClassic.TTF", 16);

	std::shared_ptr<Player> pPlayer;
	std::shared_ptr <CoreSystem::Mouse> pMouse;

	Pokemon* enemyPokemon;
	AMenu* actionMenu;
};