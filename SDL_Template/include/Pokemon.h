#pragma once

#include "Graphics.h"

class Pokemon {
public:
	Pokemon() = default;
	Pokemon(const std::string& spritePath);

	Pokemon& operator=(const Pokemon& rhs);

	void DrawFrontSprite(Maths::IRect destRect);
	void DrawBackSprite(Maths::IRect destRect);

	void Attack(Pokemon& enemy);

	void Heal();

	int GetHP() const;
	bool IsDead() const;

private:
	std::shared_ptr<GraphicsEngine::Graphics> pGfx;
	GraphicsEngine::Sprite sprite;
	int hp;
	int	att;
	int def;
	int lvl;
};