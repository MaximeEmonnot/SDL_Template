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
		Animation(Maths::IRect rect, int count, Sprite& sprite, float holdTime);
		~Animation();

		void					Draw(Maths::IRect rect);

		void					Update(float dt);
		void					ResetAnimation();

		int						GetWidth() const;
		int						GetHeight() const;
	private:
		void					Advance();
	private:
		std::shared_ptr<Graphics> gfx;
		const Sprite& sprite;
		std::vector<Maths::IRect>	frames;
		float					curFrameTime = 0.0f;
		float					holdTime;
		int						iCurFrame = 0;
		int						width;
		int						height;
	};
}

