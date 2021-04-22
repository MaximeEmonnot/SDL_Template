#pragma once
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/document.h>
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
	Character(const char* path, Graphics& gfx, SDL_Rect rect);

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

