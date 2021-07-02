#pragma once
#include "JSONParser.h"
#include "Animation.h"
class Character
{
public:
	Character(Maths::IRect rect);

	void					Draw();
	void					Update(float dt);

	Maths::IRect			GetRect() const;

protected:
	Maths::IRect							mRect;
	GraphicsEngine::Sprite					mSprite;
	std::vector<GraphicsEngine::Animation>	mAnimations;
	int										miCurSequence = 0;
};
