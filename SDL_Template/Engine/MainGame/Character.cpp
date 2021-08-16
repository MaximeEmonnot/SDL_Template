#include "Character.h"
#include <cassert>

Character::Character(Maths::IRect rect)
	:
	mRect(rect),
	mReflectionRect(Maths::IRect(rect.rect.x, rect.rect.y + rect.rect.w, rect.rect.w, rect.rect.h))
{
}

void Character::Draw()
{
	mAnimations[miCurSequence].Draw(mRect, drawPriority);
}

void Character::DrawReflection()
{
	mAnimations[miCurSequence].DrawReflection(mReflectionRect);
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
