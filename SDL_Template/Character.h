#pragma once
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include "Animation.h"
class Character
{
public:
	Character(const char* path, Graphics& gfx);

	void					Draw(SDL_Rect rect, Graphics& gfx);
	void					Update(float dt);

	void					NextAnimation();
	void					LastAnimation();

private:
	Surface					sprite;
	std::vector<Animation>	animations;
	int						iCurSequence = 0;
};

