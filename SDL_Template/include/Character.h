#pragma once
#include "JSONParser.h"
#include "Animation.h"
class Character
{
public:
	Character(Maths::IRect rect);
	virtual ~Character() = default;

	void					Update(float dt);
	void					Draw();

	void                    BlendSpriteTo(GraphicsEngine::Color c);

	Maths::IRect			GetRect() const;

protected:
	Maths::IRect							mRect;
	GraphicsEngine::Sprite					mSprite;
	std::vector<GraphicsEngine::Animation>	mAnimations;
	int										miCurSequence = 0;
	int                                     drawPriority = 2;
};
