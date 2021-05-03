#pragma once
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/document.h>
#include <fstream>
#include "Animation.h"
class Character
{
public:
	Character(GraphicsEngine::Graphics& gfx, SDL_Rect rect);

	void					Draw(GraphicsEngine::Graphics& gfx);
	void					Update(float dt);

	void                    SetPos(const IVec2D pos);
	void					NextAnimation();
	void					LastAnimation();
	void					ZoomIn();
	void					ZoomOut();

	SDL_Rect				GetRect() const;

private:
	SDL_Rect				rect;
	GraphicsEngine::Sprite					sprite;
	std::vector<GraphicsEngine::Animation>	animations;
	int						iCurSequence = 0;
};

