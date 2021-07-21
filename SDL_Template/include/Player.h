#pragma once
#include "Character.h"
#include "Keyboard.h"
#include "Pokemon.h"

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

	void InitFromJSON();
	void SaveJSON();

	void Move();

	void SetPokemon(Pokemon& pkmn);
	Pokemon& GetPokemon();

	Maths::IVec2D GetLookingDirection() const;

	void DrawPokemon();

	void TEST_PickUpItem(const std::pair<Maths::IVec2D, std::string>& item);
	void TEST_UseItem(int index);

	std::unordered_map<Maths::IVec2D, std::string, Maths::IVec2D::Hash> GetItemList() const;

public:
	bool TEST_bInitFromJSON = false;

private:
	Pokemon pokemon;

	Maths::IVec2D worldPosition;

	std::unordered_map<Maths::IVec2D, std::string, Maths::IVec2D::Hash> items;
	std::shared_ptr<CoreSystem::Keyboard> pKbd;
	Maths::IVec2D velocity;
	Maths::IVec2D lookingDirection;
	float speed = 2.0f;
};