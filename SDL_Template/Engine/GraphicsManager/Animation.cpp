#include "Animation.h"

Animation::Animation(SDL_Rect rect, int count, Sprite& sprite, float holdTime)
	:
	sprite(sprite),
	holdTime(holdTime),
	width(rect.w),
	height(rect.h)
{
	for (int i = 0; i < count; i++) {
		frames.push_back({ rect.x + rect.w * i, rect.y, rect.w, rect.h });
	}
}

void Animation::Draw(SDL_Rect rect, Graphics& gfx)
{
	gfx.DrawSprite(rect, frames[iCurFrame], sprite);
}

void Animation::Update(float dt)
{
	curFrameTime += dt;
	while (curFrameTime >= holdTime) {
		Advance();
		curFrameTime -= holdTime;
	}
}

void Animation::ResetAnimation()
{
	iCurFrame = 0;
}

int Animation::GetWidth() const
{
	return width;
}

int Animation::GetHeight() const
{
	return height;
}

void Animation::Advance()
{
	if (++iCurFrame >= frames.size()) {
		iCurFrame = 0;
	}
}