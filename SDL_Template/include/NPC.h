#pragma once
#include "Character.h"

#include "AIStanding.h"
#include "AIWalking.h"

class NPC : public Character {
private:
	friend class AI;
	friend class AIStanding;
	friend class AIWalking;

	friend class World;
private:
	enum class AnimationList {
		WalkingLeft,
		WalkingUp,
		WalkingRight,
		WalkingDown,
		StandingLeft,
		StandingUp,
		StandingRight,
		StandingDown,
		RunningLeft,
		RunningUp,
		RunningRight,
		RunningDown,
		BikingLeft,
		BikingUp,
		BikingRight,
		BikingDown,
		StandingBikeLeft,
		StandingBikeUp,
		StandingBikeRight,
		StandingBikeDown,
		OnWaterLeft,
		OnWaterUp,
		OnWaterRight,
		OnWaterDown
	};
public:
	NPC() = default;
	NPC(Maths::IRect rect, Maths::IVec2D pos, const std::string& animFile, std::unique_ptr<AI> AI);
	NPC(Maths::IRect rect, const std::string& animFile);
	~NPC() override;

	void Move(int newXOffset, int newYOffset);
	void Talk(Maths::IVec2D talkerPos);
	void StopTalking();

	void UpdateAI();

private:
	void SetPosition(const Maths::IVec2D& pos);
	void SetAnimation(int anim);
private:
	bool bIsControlledByAI = true;
	Maths::IVec2D localPos;
	std::unique_ptr<AI> pAI;
};