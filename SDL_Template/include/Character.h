#pragma once
#include "JSONParser.h"
#include "Animation.h"
class Character
{
public:
	Character(Maths::IRect rect);

	void					Draw();
	void					Update(float dt);

	void					NextAnimation();
	void					LastAnimation();

	Maths::IRect			GetRect() const;

private:
	Maths::IRect							mRect;
	GraphicsEngine::Sprite					mSprite;
	std::vector<GraphicsEngine::Animation>	mAnimations;
	int										miCurSequence = 0;
};
