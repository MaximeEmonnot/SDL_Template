#pragma once
#include "Scene.h"
#include "Player.h"

#include "TImerManager.h"

#include "PokemonMenu.h"
#include "AbilityMenu.h"
#include "ItemTypeMenu.h"
#include "ItemMenu.h"
#include "ActionMenu.h"

class FightingScene : public Scene {
private:
	enum class FightingState {
		ChoosingAction,
		ChoosingAbility,
		ChoosingItemType,
		ChoosingConsumable,
		ChoosingBall,
		ChoosingPokemon,
		PlayerFighting,
		PlayerHealing,
		PlayerCatching,
		PlayerSwitching,
		PlayerFleeing,
		EnemyAttacking,
		EnemyCatched
	};
public:
	FightingScene();
	~FightingScene();

	void Update() override;
	void Draw() override;

private:
	Pokemon* CreateRandomPokemon();
	void InitializeAbilityList();
	
	void PlayAnimation(Maths::IRect& rect);

	void Fight();
	void Heal();
	void Switch();
	void Capture();
	void Flee();

private:
	//Map with abilities and respective probabilities
	std::unordered_map<Pokemon::Ability, int, Pokemon::Ability::Hash> abilityList;

	TimerManager attackTimer;
	TimerManager fleeTimer;
	TimerManager animationTimer;

	int currentDir = 1;

	int chosenBall = -1;

	FightingState state = FightingState::ChoosingAction;

	Pokemon* enemyPokemon;
	Maths::IRect enemyPkmnRect;
	Maths::IRect playerPkmnRect;

	std::unique_ptr<AMenu> actionMenu;
	std::unique_ptr<AMenu> itemTypeMenu;
	std::unique_ptr<AMenu> consumableMenu;
	std::unique_ptr<AMenu> ballMenu;
	std::unique_ptr<AMenu> abilityMenu;
	std::unique_ptr<AMenu> pokemonMenu;

	TextBox text;
	TextBox playerPkmnStatus;
	TextBox enemyPkmnStatus;

};