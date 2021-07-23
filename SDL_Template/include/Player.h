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

	void SetPokemon(Pokemon& pkmn);
	Pokemon& GetPokemon();

	Maths::IVec2D GetLookingDirection() const;

	void DrawPokemon();

	void TEST_PickUpItem(std::shared_ptr<Item> item);
	void TEST_UseItem(int index);

	std::map<std::shared_ptr<Item>, int> GetItemList() const;

public:
	bool TEST_bInitFromJSON = false;

private:
	Pokemon pokemon;

	Maths::IVec2D worldPosition;

	std::map<std::shared_ptr<Item>, int> items;
	std::shared_ptr<CoreSystem::Keyboard> pKbd;
	Maths::IVec2D velocity;
	Maths::IVec2D lookingDirection;
	float speed = 2.0f;
};