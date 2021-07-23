#pragma once
#include "Scene.h"
#include "Player.h"

#include "TImerManager.h"

#include "PokemonMenu.h"
#include "BasicMenu.h"
#include "RightMenu.h"
#include "TextBox.h"

class FightingScene : public Scene {
public:
	FightingScene();
	~FightingScene();

	void Update() override;
	void Draw() override;

private:
	Pokemon* CreateRandomPokemon();

	void PlayAnimation(Maths::IRect& rect);

	void Fight();
	void Flee();

private:
	GraphicsEngine::Font font = GraphicsEngine::Font("ttf/arcadeClassic.TTF", 16);

	std::shared_ptr<Player> pPlayer;
	std::shared_ptr <CoreSystem::Mouse> pMouse;

	TimerManager attackTimer;
	TimerManager fleeTimer;
	TimerManager animationTimer;

	int currentDir = 1;

	float time;

	bool bIsChoosingAbility = false;
	bool bIsChoosingBall = false;
	bool bIsFighting = false;
	bool bIsFleeing = false;
	bool bPlayerIsAttacking = false;
	bool bEnemyIsAttacking = false;
	
	Pokemon* enemyPokemon;
	Maths::IRect enemyPkmnRect;
	Maths::IRect playerPkmnRect;

	std::unique_ptr<AMenu> actionMenu;
	std::unique_ptr<AMenu> pokemonMenu;

	TextBox attackPlayer;
	TextBox attackEnemy;
	TextBox fleePlayer;
};