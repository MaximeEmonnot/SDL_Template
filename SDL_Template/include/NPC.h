#pragma once
#include "Character.h"

#include "AIStanding.h"
#include "AIWalking.h"

class NPC : public Character {
private:
	friend class AI;
	friend class AIStanding;
	friend class AIWalking;
private:
	enum class AnimationList {
		StandingLeft,
		StandingUp,
		StandingRight,
		StandingDown,
		WalkingLeft,
		WalkingUp,
		WalkingRight,
		WalkingDown
	};
public:
	NPC() = default;
	NPC(Maths::IRect rect, Maths::IVec2D pos, const std::string& animFile, std::unique_ptr<AI> AI);
	~NPC() override;

	void Move(int newXOffset, int newYOffset);
	void Talk(Maths::IVec2D talkerPos);
	void StopTalking();

	void UpdateAI();

private:
	bool bIsControlledByAI = true;
	Maths::IVec2D localPos;
	std::unique_ptr<AI> pAI;
};