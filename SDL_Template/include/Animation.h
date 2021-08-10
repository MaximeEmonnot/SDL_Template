#pragma once
#include <SDL.h>
#include <vector>
#include "Graphics.h"
#include "Sprite.h"
#include "Vec2D.h"


namespace GraphicsEngine {
	class Animation
	{
	public:
		Animation(Maths::IRect rect, int count, Sprite& rSprite, float holdTime);

		void					Draw(Maths::IRect rect, int priority = 0);

		void					Update(float dt);
		void					ResetAnimation();

		int						GetWidth() const;
		int						GetHeight() const;
	private:
		void					Advance();
	private:
		std::shared_ptr<Graphics>	mpGfx;
		const Sprite&				mrSprite;
		std::vector<Maths::IRect>	mFrames;
		float						mCurFrameTime = 0.0f;
		float						mHoldTime;
		int							miCurFrame = 0;
		int							mWidth;
		int							mHeight;
	};
}

