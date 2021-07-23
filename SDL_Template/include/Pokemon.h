#pragma once

#include "Graphics.h"
#include "JSONParser.h"

class Pokemon {
private:
	friend class Player;
private:
	struct Ability {
		std::string name;
		int power;
		int maxPP;
		int currentPP;
	};
public:
	Pokemon() = default;
	Pokemon(const std::string& spritePath, int id);

	Pokemon& operator=(const Pokemon& rhs);

	void DrawFrontSprite(Maths::IRect destRect);
	void DrawBackSprite(Maths::IRect destRect);

	void Attack(Pokemon& enemy);

	void Heal(int value);

	void BoostAttack(int value);
	void BoostDefense(int value);
	void BoostHP(int value);
	void BoostLVL(int value);

	int GetHP() const;
	bool IsDead() const;

	Pokemon::Ability GetAbility(int index) const;

private:
	Ability firstAbility = {"First attack", 50, 10, 10};
	Ability secondAbility = {"Fireball", 60, 10, 10};
	Ability thirdAbility = {"CHARGE", 80, 10, 10};
	Ability fourthAbility = {"BOOOOm", 25, 10, 10};
	
	std::shared_ptr<GraphicsEngine::Graphics> pGfx;
	GraphicsEngine::Sprite sprite;

	int hp;
	int	att;
	int def;
	int lvl;
	int id;

	int currentMaxHP;
	const int maxHP = 999;
	const int maxAtt = 999;
	const int maxDef = 999;
	const int maxLvl = 100;
};