#pragma once

#include "AI.h"

#include "TImerManager.h"
#include <random>

class AIStanding : public AI {
public:
	AIStanding();
	~AIStanding() override;

	void Update(class NPC& npc) override;
private:
	TimerManager timer;
	std::mt19937 rng;
	std::uniform_real_distribution<float> dist;
	std::uniform_int_distribution<int> animDist;
};