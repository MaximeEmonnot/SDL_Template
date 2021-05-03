#pragma once
#include <SDL_ttf.h>
#include "Graphics.h"

class Font
{
public:
	Font(const char* path, int size, Graphics& gfx);
	~Font();

	// Draws text at a given position.
	void        DrawText(IVec2D pos, const char* text, SDL_Color c = { 255, 255, 255, 255 });
	void		DrawText(int x, int y, const char* text, SDL_Color c = { 255, 255, 255, 255 }) {
		DrawText(IVec2D(x, y), text, c);
	}
private:
	Graphics&	gfx;
	TTF_Font*	font;
};
