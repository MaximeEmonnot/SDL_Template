#include "Pokemon.h"
#include <random>

Pokemon::Pokemon(const std::string& spritePath, const std::string& name, int id, Type type)
	:
	pGfx(GraphicsEngine::Graphics::GetInstance()),
	name(name),
	currentMaxHP(20),
	hp(20),
	att(5),
	def(3),
	lvl(1),
	id(id),
	type(type),
	sprite(spritePath)
{
}

Pokemon& Pokemon::operator=(const Pokemon& rhs)
{
	pGfx = GraphicsEngine::Graphics::GetInstance();
	type = rhs.type;
	name = rhs.name;
	sprite = rhs.sprite;
	hp = rhs.hp;
	currentMaxHP = rhs.currentMaxHP;
	att = rhs.att;
	def = rhs.def;
	lvl = rhs.lvl;
	id = rhs.id;
	abilities = rhs.abilities;
	return *this;
}

void Pokemon::LoadAbility(Pokemon::Ability ability)
{
	abilities.push_back(ability);
}

void Pokemon::InitAbilities(std::map<int, Ability> abilityList)
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> nAttacks(2, 4);
	std::uniform_int_distribution<int> probAttack(0, 100);

	int maxAttacks = nAttacks(rng);
	while (abilities.size() < maxAttacks) {
		Ability newAbility;
		for (auto& entry : abilityList) {
			if (probAttack(rng) <= entry.first) {
				newAbility = entry.second;
				break;
			}
		}
		if (std::find(abilities.begin(), abilities.end(), newAbility) == abilities.end()) {
			abilities.push_back(newAbility);
		}
	}
}

void Pokemon::DrawFrontSprite(Maths::IRect destRect)
{
	pGfx->DrawSprite(destRect, Maths::IRect(0, 0, 56, 56), sprite);
}

void Pokemon::DrawBackSprite(Maths::IRect destRect)
{
	pGfx->DrawSprite(destRect, Maths::IRect(0, 56, 56, 56), sprite);
}

void Pokemon::Attack(Pokemon& enemy)
{
	enemy.hp -= (att - enemy.def);
}

void Pokemon::Heal(int value)
{
	hp = (hp + value > currentMaxHP) ? currentMaxHP : hp + value;
}

Maths::IVec2D Pokemon::GetHP() const
{
	return Maths::IVec2D(hp, currentMaxHP);
}
bool Pokemon::IsDead() const
{
	return hp <= 0;
}

void Pokemon::BoostAttack(int value)
{
	att = (att + value > maxAtt) ? maxAtt : att + value;
}


void Pokemon::BoostDefense(int value)
{
	def = (def + value > maxDef) ? maxDef : def + value;
}

void Pokemon::BoostHP(int value)
{
	currentMaxHP = (currentMaxHP + value > maxHP) ? maxHP : currentMaxHP + value;
}

void Pokemon::BoostLVL(int value)
{
	lvl = (lvl + value > maxLvl) ? maxLvl : lvl + value;
}

std::string Pokemon::GetName() const
{
	return name;
}

bool Pokemon::operator==(const Pokemon& rhs) const
{
	return
		(name == rhs.name) &&
		(hp == rhs.hp) &&
		(currentMaxHP == rhs.currentMaxHP) &&
		(att == rhs.att) &&
		(def == rhs.def) && 
		(lvl == rhs.lvl) &&
		(id == rhs.id);
}

bool Pokemon::operator!=(const Pokemon& rhs) const
{
	return !(*this == rhs);
}

Pokemon::Type Pokemon::GetType() const
{
	return type;
}

std::vector<Pokemon::Ability> Pokemon::GetAbilities() const
{
	return abilities;
}

Pokemon::Ability::Ability(const std::string& name, int pow, int PP, Type type)
	:
	type(type),
	name(name),
	power(pow),
	maxPP(PP),
	currentPP(PP)
{
}

Pokemon::Ability& Pokemon::Ability::operator=(const Ability& rhs)
{
	type = rhs.type;
	name = rhs.name;
	power = rhs.power;
	maxPP = rhs.maxPP;
	currentPP = rhs.currentPP;
	return *this;
}

void Pokemon::Ability::UseAbility()
{
	currentPP--;
}

void Pokemon::Ability::SetPP(int pp)
{
	currentPP = pp;
}

Pokemon::Type Pokemon::Ability::GetType() const
{
	return type;
}

std::string Pokemon::Ability::GetName() const
{
	return name;
}

int Pokemon::Ability::GetPower() const
{
	return power;
}

int Pokemon::Ability::GetPP() const
{
	return currentPP;
}

int Pokemon::Ability::GetMaxPP() const
{
	return maxPP;
}

bool Pokemon::Ability::operator==(const Ability& rhs) const
{
	return (name == rhs.name) &&
		(power == rhs.power) &&
		(maxPP == rhs.maxPP) &&
		(type == rhs.type);
}
