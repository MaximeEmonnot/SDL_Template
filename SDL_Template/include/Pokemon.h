#pragma once

#include "Graphics.h"
#include "JSONParser.h"

class Pokemon {
private:
	friend class Player;
public:
	enum class Type {
		Fire,
		Water,
		Grass,
		Normal
	};
public:
	class Ability {
	public:
		class Hash {
		public:
			size_t operator()(const Ability& ability) const {
				std::hash<std::string> hasher;
				return hasher(ability.name);
			}
		};
	public:
		Ability() = default;
		Ability(const std::string& name, int pow, int PP, Type type);

		Ability& operator=(const Ability& rhs);

		void UseAbility();
		void SetPP(int pp);

		Pokemon::Type GetType() const;
		std::string GetName() const;
		int GetPower() const;
		int GetPP() const;
		int GetMaxPP() const;

		bool operator==(const Ability& rhs) const;

	private:
		Pokemon::Type type;
		std::string name;
		int power;
		int maxPP;
		int currentPP;
	};
public:
	Pokemon() = default;
	Pokemon(const std::string& spritePath, const std::string& name, int id, Type type);

	Pokemon& operator=(const Pokemon& rhs);

	void LoadAbility(Pokemon::Ability ability);
	void InitAbilities(std::map<int, Pokemon::Ability> abilityList);

	void DrawFrontSprite(Maths::IRect destRect);
	void DrawBackSprite(Maths::IRect destRect);

	void Attack(Pokemon& enemy);

	void Heal(int value);

	void BoostAttack(int value);
	void BoostDefense(int value);
	void BoostHP(int value);
	void BoostLVL(int value);

	std::string GetName() const;
	
	Maths::IVec2D GetHP() const;
	bool IsDead() const;

	Type GetType() const;
	std::vector<Pokemon::Ability> GetAbilities() const;

	bool operator==(const Pokemon& rhs) const;
	bool operator!=(const Pokemon& rhs) const;

private:
	std::vector<Ability> abilities;

	GraphicsEngine::Sprite sprite;

	Pokemon::Type type;

	std::string name;

	int currentMaxHP;
	int hp;
	int	att;
	int def;
	int lvl;
	int id;

	const int maxHP = 999;
	const int maxAtt = 999;
	const int maxDef = 999;
	const int maxLvl = 100;
};