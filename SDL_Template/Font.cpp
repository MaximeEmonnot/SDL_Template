#include "Font.h"
#include <cassert>

Font::Font(const char* path, int size, Graphics& gfx)
	:
	gfx(gfx)
{
	TTF_Init();
	font = TTF_OpenFont(path, size);
}

Font::~Font()
{
	TTF_CloseFont(font);
}

void Font::DrawText(int x, int y, const char* text, SDL_Color c)
{
	assert(sText.GetTexture() == nullptr);
	SDL_Surface* textSurf = TTF_RenderText_Solid(font, text, c);
	Surface temp = Surface(textSurf, gfx.GetRenderer(), textSurf->w, textSurf->h);
	gfx.DrawSprite({ x, y, textSurf->w, textSurf->h }, temp);
	SDL_FreeSurface(textSurf);
}
