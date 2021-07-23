#include "Consumable.h"

Consumable::Consumable(const std::string& name, int ID, int bonusValue)
	:
	Item(name, ID),
	bonusValue(bonusValue)
{
	functionList.push_back([bonus = bonusValue](Pokemon& p) { p.Heal(bonus); printf("Healed pokemon %d!\n", bonus); });
	functionList.push_back([bonus = bonusValue](Pokemon& p) { p.Heal(bonus); printf("Healed pokemon!\n"); });
	functionList.push_back([bonus = bonusValue](Pokemon& p) { p.Heal(bonus); printf("Healed pokemon!\n"); });
	functionList.push_back([bonus = bonusValue](Pokemon& p) { p.Heal(bonus); printf("Healed pokemon!\n"); });
	functionList.push_back([bonus = bonusValue](Pokemon& p) { p.BoostAttack(bonus); printf("Added attack!\n"); });
	functionList.push_back([bonus = bonusValue](Pokemon& p) { p.BoostDefense(bonus); printf("Added defense!\n"); });
	functionList.push_back([bonus = bonusValue](Pokemon& p) { p.BoostHP(bonus); printf("Added HP!\n"); });
	functionList.push_back([bonus = bonusValue](Pokemon& p) { p.BoostLVL(bonus); printf("Added LVL!\n"); });
}

void Consumable::UseItem(Pokemon& pkmn)
{
	functionList.at(ID - 15)(pkmn);
}

int Consumable::GetBonusValue() const
{
	return bonusValue;
}
