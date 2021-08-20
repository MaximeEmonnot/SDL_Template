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
	enum class LocomotionState {
		Walking,
		Running,
		Biking,
		OnWater
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

	void InterpolatePosition();

private:
	void SetPosition(const Maths::IVec2D& pos, int locomotionState);
	void SetAnimation(int anim);
private:
	bool bIsControlledByAI = true;

	float alphaTranslation = 0.0f;
	int xAnimOffset = 0;
	int yAnimOffset = 0;
	int wAnim = 32;
	int hAnim = 44;

	Maths::IVec2D localPos;
	Maths::LLVec2D newPos = Maths::LLVec2D(0, 0);
	std::unique_ptr<AI> pAI;
};