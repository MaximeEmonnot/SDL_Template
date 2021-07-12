#pragma once
#include "Character.h"
#include "Keyboard.h"
#include "Pokemon.h"

class Player : public CoreSystem::SingletonMaker<Player>, public Character {
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

	void Move();

	void SetPokemon(Pokemon& pkmn);

	Pokemon& GetPokemon();

	void DrawPokemon();
private:
	Pokemon pokemon;

	std::shared_ptr<CoreSystem::Keyboard> pKbd;
	Maths::IVec2D velocity;
	float speed = 2.0f;
};