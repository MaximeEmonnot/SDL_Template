#pragma once
#include <SDL_ttf.h>
#include "Graphics.h"

class Font
{
public:
	Font(const char* path, int size, Graphics& gfx);
	Font(const char* path, int size, Graphics& gfx, const char* text, SDL_Color c = { 255, 255, 255, 255 });
	~Font();

	void DrawSavedText(int x, int y);
	void DrawLoadedText(int x, int y, const char* text, SDL_Color c = { 255, 255, 255, 255 });
private:
	Graphics& gfx;
	TTF_Font* font;
	Surface sText;
};

