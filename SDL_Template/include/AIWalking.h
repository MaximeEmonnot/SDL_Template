#pragma once

#include "AI.h"

#include "TImerManager.h"
#include <random>
#include <unordered_map>

class AIWalking : public AI {
public:
	AIWalking(std::unordered_map<Maths::IVec2D, int, Maths::IVec2D::Hash> tiles);
	~AIWalking() override;

	void Update(class NPC& npc) override;

private:
	bool NextTileIsObstacle(const Maths::IVec2D& nextPos, class NPC& npc);

private:
	bool bWillBeColliding = false;

	const int tileWidth = 32;
	const int tileHeight = 32;

	const Maths::IVec2D center = Maths::IVec2D(int(400 / tileWidth), int(300 / tileHeight));

	TimerManager timer;
	std::mt19937 rng;
	std::uniform_real_distribution<float> timeWalkDist;
	std::uniform_real_distribution<float> timeStandDist;
	std::uniform_int_distribution<int> animDist;
	std::unordered_map<Maths::IVec2D, int, Maths::IVec2D::Hash> houseTiles;
};