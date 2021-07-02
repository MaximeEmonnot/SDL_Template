#pragma once
#include "Character.h"

class Player : public Character {
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

	void Move(Maths::IVec2D dir);
private:
	float speed = 2.0f;
	Maths::IVec2D velocity;
};