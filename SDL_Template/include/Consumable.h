#pragma once
#include "Item.h"
#include <vector>
#include <functional>

class Consumable : public Item {
public:
	Consumable(const std::string& name, int ID, int bonusValue);

	void UseItem(Pokemon& pkmn) override;

	int GetBonusValue() const;

private:
	std::vector<std::function<void(Pokemon&)>> functionList;
	int bonusValue;
};