#include "Pokemon.h"

Pokemon::Pokemon(const std::string& spritePath, int id)
	:
	pGfx(GraphicsEngine::Graphics::GetInstance()),
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
	sprite = rhs.sprite;
	hp = rhs.hp;
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

void Pokemon::Heal()
{
	hp = 20;
}

int Pokemon::GetHP() const
{
	return hp;
}

bool Pokemon::IsDead() const
{
	return hp <= 0;
}

Pokemon::Ability Pokemon::GetAbility(int index) const
{
	switch (index) {
	case 0:
		return firstAbility;
		break;
	case 1:
		return secondAbility;
		break;
	case 2:
		return thirdAbility;
		break;
	case 3:
		return fourthAbility;
		break;
	default:
		break;
	}
}
