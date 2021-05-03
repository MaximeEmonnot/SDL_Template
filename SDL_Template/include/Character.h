#pragma once
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/document.h>
#include <fstream>
#include "Animation.h"
class Character
{
public:
	Character(Graphics& gfx, SDL_Rect rect);

	void					Draw(Graphics& gfx);
	void					Update(float dt);

	void                    SetPos(const IVec2D pos);
	void					NextAnimation();
	void					LastAnimation();
	void					ZoomIn();
	void					ZoomOut();

	SDL_Rect				GetRect() const;

private:
	SDL_Rect				rect;
	Sprite					sprite;
	std::vector<Animation>	animations;
	int						iCurSequence = 0;
};

