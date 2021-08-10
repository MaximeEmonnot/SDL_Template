#include "AIWalking.h"
#include "NPC.h"
#include "House.h"

AIWalking::AIWalking(std::unordered_map<Maths::IVec2D, int, Maths::IVec2D::Hash> tiles)
	:
	rng(std::random_device{}()),
	timeWalkDist(1.5f, 3.0f),
	timeStandDist(0.5f, 1.5f),
	animDist(0, 7),
	houseTiles(tiles)
{
	timer.ResetTimer(timeStandDist(rng));
}

AIWalking::~AIWalking()
{
	houseTiles.clear();
}

void AIWalking::Update(NPC& npc)
{
	//Update Timer
	timer.Update();
	if (timer.IsTimerDown() 
		|| bWillBeColliding
		) {
		int animSequence = npc.miCurSequence;
		while (animSequence == npc.miCurSequence) {
			animSequence = npc.miCurSequence;
			npc.miCurSequence = animDist(rng);

			switch (NPC::AnimationList(npc.miCurSequence)) {
			case NPC::AnimationList::WalkingLeft:
				if (NextTileIsObstacle(Maths::IVec2D(-16, 0), npc)) animSequence = npc.miCurSequence;
				break;
			case NPC::AnimationList::WalkingUp:
				if (NextTileIsObstacle(Maths::IVec2D(0, -20), npc)) animSequence = npc.miCurSequence;
				break;
			case NPC::AnimationList::WalkingRight:
				if (NextTileIsObstacle(Maths::IVec2D(16, 0), npc)) animSequence = npc.miCurSequence;
				break;
			case NPC::AnimationList::WalkingDown:
				if (NextTileIsObstacle(Maths::IVec2D(0, 20), npc)) animSequence = npc.miCurSequence;
				break;
			}
		}
		bWillBeColliding = false;
		if (npc.miCurSequence < 4) {
			timer.ResetTimer(timeStandDist(rng));
		}
		else {
			timer.ResetTimer(timeWalkDist(rng));
		}
	}

	switch (NPC::AnimationList(npc.miCurSequence)) {
	case NPC::AnimationList::WalkingLeft:
		if (!NextTileIsObstacle(Maths::IVec2D(-16, 0), npc)) {
			npc.localPos.x -= 2;
			npc.mRect.rect.x -= 2;
		}
		else bWillBeColliding = true;
		break;
	case NPC::AnimationList::WalkingUp:
		if (!NextTileIsObstacle(Maths::IVec2D(0, -20), npc)) {
			npc.localPos.y -= 2;
			npc.mRect.rect.y -= 2;
		}
		else bWillBeColliding = true;
		break;
	case NPC::AnimationList::WalkingRight:
		if (!NextTileIsObstacle(Maths::IVec2D(16, 0), npc)) {
			npc.localPos.x += 2;
			npc.mRect.rect.x += 2;
		}
		else bWillBeColliding = true;
		break;
	case NPC::AnimationList::WalkingDown:
		if (!NextTileIsObstacle(Maths::IVec2D(0, 20), npc)) {
			npc.localPos.y += 2;
			npc.mRect.rect.y += 2;
		}
		else bWillBeColliding = true;
		break;
	default:
		break;
	}
}

bool AIWalking::NextTileIsObstacle(const Maths::IVec2D& nextPos, NPC& npc)
{
	Maths::IVec2D pos = Maths::IVec2D(int((npc.localPos.x + nextPos.x) / tileWidth), int((npc.localPos.y + nextPos.y) / tileHeight));
	Maths::IVec2D nextWorldPos = Maths::IVec2D(int((npc.GetRect().GetCenterOfRect().x + nextPos.x) / tileWidth), int((npc.GetRect().GetCenterOfRect().y + nextPos.y) / tileHeight));
	auto itr = houseTiles.find(pos);
	if (itr != houseTiles.end()) {
		return (itr->second & House::TileTypes::FlowerPot) == House::TileTypes::FlowerPot ||
			(itr->second & House::TileTypes::Flower) == House::TileTypes::Flower ||
			nextWorldPos == center ||
			!((itr->second == House::TileTypes::ChairL) ||
			(itr->second == House::TileTypes::ChairR) ||
			(itr->second == House::TileTypes::Carpet0) ||
			(itr->second == House::TileTypes::Carpet1) ||
			((itr->second & House::TileTypes::Floor0) == House::TileTypes::Floor0 ||
			(itr->second & House::TileTypes::Floor1) == House::TileTypes::Floor1 ||
			(itr->second & House::TileTypes::Floor2) == House::TileTypes::Floor2));
	}
	return true;
}