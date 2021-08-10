#include "Character.h"
#include <cassert>

Character::Character(Maths::IRect rect)
	:
	mRect(rect)
{
}

void Character::Draw()
{
	mAnimations[miCurSequence].Draw(mRect, drawPriority);
}

void Character::BlendSpriteTo(GraphicsEngine::Color c)
{
	mSprite.BlendColor(c);
}

void Character::Update(float dt)
{
	mAnimations[miCurSequence].Update(dt);
}

Maths::IRect Character::GetRect() const
{
	return mRect;
}
