#include "Character.h"
#include <cassert>

Character::Character(GraphicsEngine::Graphics& gfx, SDL_Rect rect)
	:
	rect(rect)
{
	std::ifstream file;
	file.open("json\\kirby.json");
	if (!file)
		throw SDLException("SDL JSON Exception caught", __FILE__, __LINE__, "An error has been caught during JSON File Opening.\nPlease check JSON file path.");
	rapidjson::IStreamWrapper isw(file);
	rapidjson::Document doc;
	doc.ParseStream(isw);
	file.close();
	sprite.InitSurface(doc["filename"].GetString(), gfx.GetRenderer());

	rapidjson::Value& v = doc["animations"];

	for (rapidjson::Value::MemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr) {
		animations.push_back(GraphicsEngine::Animation({ itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt(), itr->value.GetArray()[2].GetInt(), itr->value.GetArray()[3].GetInt() },
											itr->value.GetArray()[4].GetInt(), sprite, itr->value.GetArray()[5].GetFloat()));
	}
	int test = 0;
}

void Character::Draw(GraphicsEngine::Graphics& gfx)
{
	animations[iCurSequence].Draw(rect, gfx);
}

void Character::Update(float dt)
{
	animations[iCurSequence].Update(dt);
}

void Character::SetPos(const IVec2D pos)
{
	rect.x = pos.x - rect.w / 2;
	rect.y = pos.y - rect.h / 2;
}

void Character::NextAnimation()
{
	animations[iCurSequence].ResetAnimation();
	if (iCurSequence == animations.size() - 1) {
		iCurSequence = 0;
	}
	else {
		iCurSequence++;
	}
}

void Character::LastAnimation()
{
	animations[iCurSequence].ResetAnimation();
	if (iCurSequence == 0) {
		iCurSequence = (int)animations.size() - 1;
	}
	else {
		iCurSequence--;
	}
}

void Character::ZoomIn()
{
	rect.w *= 2;
	rect.h *= 2;
}

void Character::ZoomOut()
{
	rect.w /= 2;
	rect.h /= 2;
}

SDL_Rect Character::GetRect() const
{
	return rect;
}
