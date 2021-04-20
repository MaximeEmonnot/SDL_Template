#pragma once
#include <SDL.h>
#include <vector>
#include "Surface.h"
#include "Graphics.h"
#include "Vec2D.h"

class Animation
{
public:
	Animation(SDL_Rect rect, int count, Surface& sprite, float holdTime);

	void Draw(SDL_Rect rect, Graphics& gfx);

	void Update(float dt);
	void ResetAnimation();

	int GetWidth() const;
	int GetHeight() const;
private:
	void Advance();
private:
	const Surface& sprite;
	std::vector<SDL_Rect> frames;
	int iCurFrame = 0;
	float holdTime;
	float curFrameTime = 0.0f;
	int width;
	int height;
};

