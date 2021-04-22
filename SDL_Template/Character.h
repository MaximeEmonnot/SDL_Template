#pragma once
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include "Animation.h"
class Character
{
private:
	class Exception : public SDLException {
	public:
		Exception(const std::string& file, unsigned int line, const std::string& note) noexcept
			:
			SDLException(file, line, note)
		{}
		inline std::string GetType() const noexcept override {
			return "SDL JSON Exception caught";
		}
	};
public:
	Character(const char* path, Graphics& gfx);

	void					Draw(SDL_Rect rect, Graphics& gfx);
	void					Update(float dt);

	void					NextAnimation();
	void					LastAnimation();

private:
	Sprite					sprite;
	std::vector<Animation>	animations;
	int						iCurSequence = 0;
};

