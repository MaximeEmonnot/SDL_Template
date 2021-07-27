#pragma once
#include "Character.h"
#include "Keyboard.h"
#include "Pokemon.h"

#include "Consumable.h"
#include "Ball.h"
#include <map>


class Player : public CoreSystem::SingletonMaker<Player>, public Character {
private:
	friend class ExplorationScene;
private:
	enum class AnimationList {
		WalkingLeft,
		WalkingUp,
		WalkingRight,
		WalkingDown,
		StandingLeft,
		StandingUp,
		StandingRight,
		StandingDown
	};
public:
	Player(Maths::IRect rect, const std::string& animFile);
	~Player() override;

	void InitFromJSON();
	void SaveJSON();

	void Move();

	void AddPokemon(Pokemon& pkmn);
	void SetFirstPokemon(int index);
	Pokemon& GetPokemon();
	std::vector<Pokemon> GetPokemonList() const;

	Maths::IVec2D GetLookingDirection() const;

	void DrawPokemon();

	bool TEST_CapturePokemon(int index, Pokemon& pkmn);

	void TEST_PickUpItem(std::shared_ptr<Item> item);
	void TEST_UseItem(int indexItem) {
		TEST_UseItem(indexItem, selectedPokemon - pokemon.begin());
	}
	void TEST_UseItem(int indexItem, int indexPkmn);

	bool TEST_CanUseItem(int index);

	std::map<std::shared_ptr<Item>, int> GetItemList() const;

public:
	bool TEST_bInitFromJSON = false;

private:

	std::vector<Pokemon>::iterator selectedPokemon;

	std::vector<Pokemon> pokemon;

	Maths::IVec2D worldPosition;

	std::map<std::shared_ptr<Item>, int> items;
	std::shared_ptr<CoreSystem::Keyboard> pKbd;
	Maths::IVec2D velocity;
	Maths::IVec2D lookingDirection;
	float speed = 2.0f;
};