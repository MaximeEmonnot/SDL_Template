#include "Animation.h"

GraphicsEngine::Animation::Animation(Maths::IRect rect, int count, Sprite& sprite, float holdTime)
	:
	gfx(Graphics::GetInstance()),
	sprite(sprite),
	holdTime(holdTime),
	width(rect.rect.w),
	height(rect.rect.h)
{
	for (int i = 0; i < count; i++) {
		frames.push_back({ rect.rect.x + rect.rect.w * i, rect.rect.y, rect.rect.w, rect.rect.h });
	}
}

void GraphicsEngine::Animation::Draw(Maths::IRect rect)
{
	gfx->DrawSprite(rect, frames[iCurFrame], sprite);
}

void GraphicsEngine::Animation::Update(float dt)
{
	curFrameTime += dt;
	while (curFrameTime >= holdTime) {
		Advance();
		curFrameTime -= holdTime;
	}
}

void GraphicsEngine::Animation::ResetAnimation()
{
	iCurFrame = 0;
}

int GraphicsEngine::Animation::GetWidth() const
{
	return width;
}

int GraphicsEngine::Animation::GetHeight() const
{
	return height;
}

void GraphicsEngine::Animation::Advance()
{
	if (++iCurFrame >= frames.size()) {
		iCurFrame = 0;
	}
}
