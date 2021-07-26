#include "Pokemon.h"

Pokemon::Pokemon(const std::string& spritePath, const std::string& name, int id)
	:
	pGfx(GraphicsEngine::Graphics::GetInstance()),
	name(name),
	currentMaxHP(20),
	hp(20),
	att(5),
	def(3),
	lvl(1),
	id(id)
{
	sprite.InitSurface(spritePath.c_str());
}

Pokemon& Pokemon::operator=(const Pokemon& rhs)
{
	pGfx = GraphicsEngine::Graphics::GetInstance();
	name = rhs.name;
	sprite = rhs.sprite;
	hp = rhs.hp;
	currentMaxHP = rhs.currentMaxHP;
	att = rhs.att;
	def = rhs.def;
	lvl = rhs.lvl;
	id = rhs.id;
	return *this;
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

int Pokemon::GetHP() const
{
	return hp;
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

Pokemon::Ability Pokemon::GetAbility(int index) const
{

	switch (index) {
	case 0:
		return firstAbility;
	case 1:
		return secondAbility;
	case 2:
		return thirdAbility;
	case 3:
		return fourthAbility;
	default:
		throw EngineException("Pokemon Ability NULL", __FILE__, "An exception occured.\nPokemon ability was out of range.", __LINE__);
	}
}
