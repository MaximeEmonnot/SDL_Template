#include "Font.h"

GraphicsEngine::Font::Font(const char* path, int size, Graphics& gfx)
	:
	gfx(gfx)
{
	if (TTF_Init() < 0)
		throw SDLException("SDL TTF Exception caught", __FILE__, __LINE__, "An error has been caught during TTF Initialisation.");
	font = TTF_OpenFont(path, size);
	if (font == NULL)
		throw SDLException("SDL TTF Exception caught", __FILE__, __LINE__, "An error has been caught during TTF Font Opening.\nPlease check TTF file path.");
}

GraphicsEngine::Font::~Font()
{
	TTF_CloseFont(font);
}

void GraphicsEngine::Font::DrawText(IVec2D pos, const char* text, SDL_Color c)
{
	SDL_Surface* textSurf = TTF_RenderText_Solid(font, text, c);
	Sprite tSprite = Sprite(textSurf, gfx.GetRenderer(), textSurf->w, textSurf->h);
	gfx.DrawSprite({ pos.x, pos.y, textSurf->w, textSurf->h }, tSprite);
	SDL_FreeSurface(textSurf);
}
