#pragma once
#include "Character.h"

#include "AIStanding.h"

class NPC : public Character {
private:
	friend class AI;
	friend class AIStanding;
private:
	enum class AnimationList {
		StandingLeft,
		StandingUp,
		StandingRight,
		StandingDown
	};
public:
	NPC() = default;
	NPC(Maths::IRect rect, const std::string& animFile, std::unique_ptr<AI> AI);

	void Move(int newXOffset, int newYOffset);

	void UpdateAI();

private:
	std::unique_ptr<AI> pAI;
};