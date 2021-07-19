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


	//Modifier pour Timer (float en interne)
	TimerManager timer;
	float attackTimer = 1.5f;
	float fleeTimer = 2.0f;

	bool bIsChoosingAbility = false;
	bool bIsFighting = false;
	bool bIsFleeing = false;
	bool bPlayerIsAttacking = false;
	bool bEnemyIsAttacking = false;
	
	Pokemon* enemyPokemon;
	std::unique_ptr<AMenu> actionMenu;
	std::unique_ptr<AMenu> pokemonMenu;

	TextBox attackPlayer;
	TextBox attackEnemy;
	TextBox fleePlayer;
};