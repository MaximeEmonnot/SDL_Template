#include "AIStanding.h"
#include "NPC.h"

AIStanding::AIStanding()
	:
	rng(std::random_device{}()),
	dist(2.5f, 5.0f),
	animDist(0, 3)
{
	timer.ResetTimer(dist(rng));
}

void AIStanding::Update(NPC& npc)
{
	timer.Update();
	if (timer.IsTimerDown()) {
		int animSequence = npc.miCurSequence;
		while (animSequence == npc.miCurSequence) {
			animSequence = npc.miCurSequence;
			npc.miCurSequence = animDist(rng);
		}
		timer.ResetTimer(dist(rng));
	}
}
