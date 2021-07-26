#pragma once
#include "Scene.h"
#include "Player.h"

#include "TImerManager.h"

#include "PokemonMenu.h"
#include "ItemTypeMenu.h"
#include "ItemMenu.h"
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
	void Heal();
	void Capture();
	void Flee();

private:
	std::shared_ptr<Player> pPlayer;
	std::shared_ptr <CoreSystem::Mouse> pMouse;

	TimerManager attackTimer;
	TimerManager fleeTimer;
	TimerManager animationTimer;

	int currentDir = 1;

	int chosenItem = -1;
	int chosenBall = -1;

	float time;

	bool bIsChoosingAbility = false;
	bool bIsChoosingItemType = false;
	bool bIsChoosingItem = false;
	bool bIsChoosingBall = false;
	bool bIsFighting = false;
	bool bIsHealing = false;
	bool bIsCatching = false;
	bool bIsFleeing = false;
	bool bPlayerIsCatching = false;
	bool bPlayerIsAttacking = false;
	bool bPlayerIsHealing = false;
	bool bEnemyIsAttacking = false;
	bool bEnemyIsCatched = false;
	
	Pokemon* enemyPokemon;
	Maths::IRect enemyPkmnRect;
	Maths::IRect playerPkmnRect;

	std::unique_ptr<AMenu> actionMenu;
	std::unique_ptr<AMenu> itemTypeMenu;
	std::unique_ptr<AMenu> consumableMenu;
	std::unique_ptr<AMenu> ballMenu;
	std::unique_ptr<AMenu> pokemonMenu;

	TextBox text;

};