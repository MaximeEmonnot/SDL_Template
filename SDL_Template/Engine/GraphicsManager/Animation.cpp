#include "Animation.h"

GraphicsEngine::Animation::Animation(Maths::IRect rect, int count, Sprite& sprite, float holdTime)
	:
	mpGfx(Graphics::GetInstance()),
	mrSprite(sprite),
	mHoldTime(holdTime),
	mWidth(rect.rect.w),
	mHeight(rect.rect.h)
{
	for (int i = 0; i < count; i++) {
		mFrames.push_back({ rect.rect.x + rect.rect.w * i, rect.rect.y, rect.rect.w, rect.rect.h });
	}
}

void GraphicsEngine::Animation::Draw(Maths::IRect rect, int priority)
{
	mpGfx->DrawSprite(rect, mFrames[miCurFrame], mrSprite, priority);
}

void GraphicsEngine::Animation::Update(float dt)
{
	mCurFrameTime += dt;
	while (mCurFrameTime >= mHoldTime) {
		Advance();
		mCurFrameTime -= mHoldTime;
	}
}

void GraphicsEngine::Animation::ResetAnimation()
{
	miCurFrame = 0;
}

int GraphicsEngine::Animation::GetWidth() const
{
	return mWidth;
}

int GraphicsEngine::Animation::GetHeight() const
{
	return mHeight;
}

void GraphicsEngine::Animation::Advance()
{
	if (++miCurFrame >= mFrames.size()) {
		miCurFrame = 0;
	}
}
