#include "NPC.h"

NPC::NPC(Maths::IRect rect, Maths::IVec2D pos, const std::string& animFile, std::unique_ptr<AI> AI)
	:
	Character(rect),
	pAI(std::move(AI)),
	localPos(pos)
{
	localPos.x += 16;
	localPos.y += 16;
	printf("pos npc x: %d , pos npc y: %d\n", localPos.x, localPos.y);
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

NPC::~NPC()
{
}

void NPC::Move(int newXOffset, int newYOffset)
{
	mRect.rect.x += newXOffset;
	mRect.rect.y += newYOffset;
}

void NPC::Talk(Maths::IVec2D lookingDir)
{
	bIsControlledByAI = false;

	if (lookingDir == Maths::IVec2D(1, 0)) {
		miCurSequence = int(AnimationList::StandingRight);
	}
	else if (lookingDir == Maths::IVec2D(-1, 0)) {
		miCurSequence = int(AnimationList::StandingLeft);
	}
	else if (lookingDir == Maths::IVec2D(0, 1)) {
		miCurSequence = int(AnimationList::StandingDown);
	}
	else if (lookingDir == Maths::IVec2D(0, -1)) {
		miCurSequence = int(AnimationList::StandingUp);
	}
}

void NPC::StopTalking()
{
	bIsControlledByAI = true;
}

void NPC::UpdateAI()
{
	if (bIsControlledByAI) pAI->Update(*this);
}
