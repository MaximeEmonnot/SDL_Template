#include "Player.h"

Player::Player(Maths::IRect rect, const std::string& animFile)
	:
	Character(rect)
{
	JSONParser jsonParse(animFile);

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

void Player::Move(Maths::IVec2D dir)
{
	//Movement
	mRect += dir * speed;
	
	//Animation
	if (dir != Maths::IVec2D(0,0)) {
		if (dir.y != 0) {
			if (dir.y > 0) {
				miCurSequence = (int)AnimationList::WalkingDown;
			}
			else {
				miCurSequence = (int)AnimationList::WalkingUp;
			}
		}
		else {
			if (dir.x > 0) {
				miCurSequence = (int)AnimationList::WalkingRight;
			}
			else {
				miCurSequence = (int)AnimationList::WalkingLeft;
			}
		}
	}
	else {
		if (velocity != Maths::IVec2D(0, 0)) {
			if (velocity.y != 0) {
				if (velocity.y > 0) {
					miCurSequence = (int)AnimationList::StandingDown;
				}
				else {
					miCurSequence = (int)AnimationList::StandingUp;
				}
			}
			else {
				if (velocity.x > 0) {
					miCurSequence = (int)AnimationList::StandingRight;
				}
				else {
					miCurSequence = (int)AnimationList::StandingLeft;
				}
			}
		}
	}

	//Current Velocity
	velocity = dir;
}