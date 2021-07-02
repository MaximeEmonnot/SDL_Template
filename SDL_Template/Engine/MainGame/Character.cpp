#include "Character.h"
#include <cassert>

Character::Character(Maths::IRect rect)
	:
	mRect(rect)
{
	JSONParser jsonParse("json/kirby.json");

	mSprite.InitSurface(jsonParse.GetValueOf("filename").GetString());

	auto& v = jsonParse.GetValueOf("animations");

	for (auto itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr) {
		mAnimations.push_back(GraphicsEngine::Animation({ itr->value.GetArray()[0].GetInt(), 
														itr->value.GetArray()[1].GetInt(), 
														itr->value.GetArray()[2].GetInt(), 
														itr->value.GetArray()[3].GetInt() },
														itr->value.GetArray()[4].GetInt(), 
														mSprite, 
														itr->value.GetArray()[5].GetFloat()));
	}
}

void Character::Draw()
{
	mAnimations[miCurSequence].Draw(mRect);
}

void Character::Update(float dt)
{
	mAnimations[miCurSequence].Update(dt);
}

void Character::NextAnimation()
{
	mAnimations[miCurSequence].ResetAnimation();
	if (miCurSequence == mAnimations.size() - 1) {
		miCurSequence = 0;
	}
	else {
		miCurSequence++;
	}
}

void Character::LastAnimation()
{
	mAnimations[miCurSequence].ResetAnimation();
	if (miCurSequence == 0) {
		miCurSequence = (int)mAnimations.size() - 1;
	}
	else {
		miCurSequence--;
	}
}

Maths::IRect Character::GetRect() const
{
	return mRect;
}
