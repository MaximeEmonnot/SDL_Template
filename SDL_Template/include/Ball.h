#pragma once
#include "Item.h"

class Ball : public Item {
public:
	Ball(const std::string& name, int ID, int probability);

	void UseItem(Pokemon& pkmn) override;

	int GetProbability() const;

private:
	int probability;
};