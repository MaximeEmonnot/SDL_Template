#include "Character.h"
#include <cassert>

Character::Character(Maths::IRect rect)
	:
	mRect(rect)
{
	
	std::ifstream file;
	file.open("json\\kirby.json");
	if (!file)
		throw SDLException("SDL JSON Exception caught", __FILE__, "An error has been caught during JSON File Opening.\nPlease check JSON file path.", __LINE__);
	rapidjson::IStreamWrapper isw(file);
	rapidjson::Document doc;
	doc.ParseStream(isw);
	file.close();

	mSprite.InitSurface(doc["filename"].GetString());

	rapidjson::Value& v = doc["animations"];

	for (rapidjson::Value::MemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr) {
		mAnimations.push_back(GraphicsEngine::Animation({ itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt(), itr->value.GetArray()[2].GetInt(), itr->value.GetArray()[3].GetInt() },
											itr->value.GetArray()[4].GetInt(), mSprite, itr->value.GetArray()[5].GetFloat()));
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
