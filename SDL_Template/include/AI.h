#pragma once
class AI {
public:
	AI() = default; 
	virtual ~AI() = default;
	virtual void Update(class NPC& npc) = 0;
};