#pragma once
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/document.h>
#include <fstream>
#include "Animation.h"
class Character
{
public:
	Character(GraphicsEngine::Graphics& gfx, Maths::IRect rect);

	void					Draw(GraphicsEngine::Graphics& gfx);
	void					Update(float dt);

	void					NextAnimation();
	void					LastAnimation();

	Maths::IRect				GetRect() const;

private:
	Maths::IRect				rect;
	GraphicsEngine::Sprite					sprite;
	std::vector<GraphicsEngine::Animation>	animations;
	int						iCurSequence = 0;
};

