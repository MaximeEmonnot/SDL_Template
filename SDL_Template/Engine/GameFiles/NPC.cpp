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

	mSprite = GraphicsEngine::Sprite(jsonParse.GetValueOf("filename").GetString());

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

	miCurSequence = static_cast<int>(AnimationList::StandingRight);
}

NPC::NPC(Maths::IRect rect, const std::string& animFile)
	:
	Character(rect)
{
	//Read json
	JSONParser::Reader jsonParse;
	jsonParse.ReadFile(animFile);

	mSprite = GraphicsEngine::Sprite(jsonParse.GetValueOf("filename").GetString());

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

	miCurSequence = static_cast<int>(AnimationList::StandingRight);
}

NPC::~NPC()
{
}

void NPC::Move(int newXOffset, int newYOffset)
{
	mRect.rect.x += newXOffset;
	mRect.rect.y += newYOffset;

	if (mRect.rect.y >= 280) drawPriority = 3;
	else drawPriority = 2;
}

void NPC::Talk(Maths::IVec2D lookingDir)
{
	bIsControlledByAI = false;

	if (lookingDir == Maths::IVec2D(1, 0)) {
		miCurSequence = static_cast<int>(AnimationList::StandingRight);
	}
	else if (lookingDir == Maths::IVec2D(-1, 0)) {
		miCurSequence = static_cast<int>(AnimationList::StandingLeft);
	}
	else if (lookingDir == Maths::IVec2D(0, 1)) {
		miCurSequence = static_cast<int>(AnimationList::StandingDown);
	}
	else if (lookingDir == Maths::IVec2D(0, -1)) {
		miCurSequence = static_cast<int>(AnimationList::StandingUp);
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

void NPC::InterpolatePosition()
{
	mRect.rect.x += static_cast<int>((newPos.x - mRect.rect.x - xAnimOffset - static_cast<int>(mRect.rect.w * 0.5)) * alphaTranslation);
	mRect.rect.y += static_cast<int>((newPos.y - mRect.rect.y - yAnimOffset - static_cast<int>(mRect.rect.h * 0.75)) * alphaTranslation);

	mReflectionRect.rect.x += static_cast<int>((newPos.x - mReflectionRect.rect.x - xAnimOffset - static_cast<int>(mReflectionRect.rect.w * 0.5)) * alphaTranslation);
	mReflectionRect.rect.y += static_cast<int>((newPos.y - mReflectionRect.rect.y + mReflectionRect.rect.w - yAnimOffset - static_cast<int>(mReflectionRect.rect.h * 0.75)) * alphaTranslation);

	if (mRect.rect.y > 300) drawPriority = 3;
	else drawPriority = 2;
}

void NPC::SetPosition(const Maths::IVec2D& pos, int locomotionState)
{
	newPos = static_cast<Maths::LLVec2D>(pos);

	switch (static_cast<LocomotionState>(locomotionState)) {
	case LocomotionState::Walking:
		wAnim = 32;
		hAnim = 44;
		xAnimOffset = 0;
		yAnimOffset = 0;
		alphaTranslation = 0.25f;
		break;
	case LocomotionState::Running:
		wAnim = 32;
		hAnim = 44;
		xAnimOffset = 0;
		yAnimOffset = 0;
		alphaTranslation = 0.50f;
		break;
	case LocomotionState::OnWater:
		alphaTranslation = 0.50f;
		wAnim = 52;
		hAnim = 58;
		xAnimOffset = 10;
		yAnimOffset = 7;
		break;
	case LocomotionState::Biking:
		alphaTranslation = 0.75f;
		wAnim = 48;
		hAnim = 48;
		xAnimOffset = 7;
		yAnimOffset = 2;
		break;
	default:
		break;
	}

	mRect.rect.w = wAnim;
	mRect.rect.h = hAnim;
	mReflectionRect.rect.w = wAnim;
	mReflectionRect.rect.h = hAnim;
}

void NPC::SetAnimation(int anim)
{
	miCurSequence = anim;
}
