#pragma once
#include <SDL_ttf.h>
#include "Graphics.h"


class Font
{
private:
	class Exception : public SDLException {
	public:
		Exception(const std::string& file, unsigned int line, const std::string& note)
			:
			SDLException(file, line, note)
		{}
		std::string GetType() const override {
			return "SDL TTF Exception caught";
		}
	};
public:
	Font(const char* path, int size, Graphics& gfx);
	~Font();

	void		DrawText(int x, int y, const char* text, SDL_Color c = { 255, 255, 255, 255 });
private:
	Graphics&	gfx;
	TTF_Font*	font;
};

