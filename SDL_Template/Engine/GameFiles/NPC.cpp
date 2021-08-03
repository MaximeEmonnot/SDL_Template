#include "NPC.h"

NPC::NPC(Maths::IRect rect, const std::string& animFile, std::unique_ptr<AI> AI)
	:
	Character(rect),
	pAI(std::move(AI))
{

	//Read json
	JSONParser::Reader jsonParse;
	jsonParse.ReadFile(animFile);

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

	miCurSequence = (int)AnimationList::StandingRight;
}

void NPC::Move(int newXOffset, int newYOffset)
{
	mRect.rect.x += newXOffset;
	mRect.rect.y += newYOffset;
}

void NPC::UpdateAI()
{
	pAI->Update(*this);
}
