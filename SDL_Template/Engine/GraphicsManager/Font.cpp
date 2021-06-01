#include "Font.h"

GraphicsEngine::Font::Font(const char* path, int size)
	:
	pGfx(Graphics::GetInstance())
{
	if (TTF_Init() < 0)
		throw SDLException("SDL TTF Exception caught", __FILE__, "An error has been caught during TTF Initialisation.", __LINE__);
	pFont = TTF_OpenFont(path, size);
	if (pFont == NULL)
		throw SDLException("SDL TTF Exception caught", __FILE__, "An error has been caught during TTF Font Opening.\nPlease check TTF file path.", __LINE__);
}

GraphicsEngine::Font::~Font()
{
	pGfx->Kill();
	TTF_CloseFont(pFont);
}

void GraphicsEngine::Font::DrawText(Maths::IVec2D pos, const char* text, GraphicsEngine::Color c)
{
	SDL_Surface* textSurf = TTF_RenderText_Solid(pFont, text, c.c);
	Sprite tSprite = Sprite(textSurf, textSurf->w, textSurf->h);
	pGfx->DrawSprite({ pos.x, pos.y, textSurf->w, textSurf->h }, tSprite);
	SDL_FreeSurface(textSurf);
}
