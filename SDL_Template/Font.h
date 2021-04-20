#pragma once
#include <SDL_ttf.h>
#include "Graphics.h"

class Font
{
public:
	Font(const char* path, int size);
	Font(const char* path, int size, Graphics& gfx, const char* text, SDL_Color c = { 255, 255, 255, 255 });
	~Font();

	void DrawSavedText(int x, int y, Graphics& gfx);
	void DrawLoadedText(int x, int y, Graphics& gfx, const char* text, SDL_Color c = { 255, 255, 255, 255 });
private:
	TTF_Font* font;
	Surface sText;
};

