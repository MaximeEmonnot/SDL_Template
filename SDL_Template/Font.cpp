#include "Font.h"
#include <cassert>

Font::Font(const char* path, int size, Graphics& gfx)
	:
	gfx(gfx)
{
	TTF_Init();
	font = TTF_OpenFont(path, size);
}

Font::Font(const char* path, int size, Graphics& gfx, const char* text, SDL_Color c)
	:
	gfx(gfx)
{
	TTF_Init();
	font = TTF_OpenFont(path, size);
	SDL_Surface* textSurf = TTF_RenderText_Solid(font, text, c);
	sText = Surface(textSurf, gfx.GetRenderer(), textSurf->w, textSurf->h);
	SDL_FreeSurface(textSurf);
}

Font::~Font()
{
	TTF_CloseFont(font);
}

void Font::DrawSavedText(int x, int y)
{
	gfx.DrawSprite({ x, y, sText.GetWidth(), sText.GetHeight() }, sText);
}

void Font::DrawLoadedText(int x, int y, const char* text, SDL_Color c)
{
	assert(sText.GetTexture() == nullptr);

	SDL_Surface* textSurf = TTF_RenderText_Solid(font, text, c);
	Surface temp = Surface(textSurf, gfx.GetRenderer(), textSurf->w, textSurf->h);
	gfx.DrawSprite({ x, y, textSurf->w, textSurf->h }, temp);
	SDL_FreeSurface(textSurf);
}
