#include "Font.h"

GraphicsEngine::Font::Font(const std::string& path, int size)
{
	if (TTF_Init() < 0)
		throw EngineException("SDL TTF Exception caught", __FILE__, "An error has been caught during TTF Initialisation.", __LINE__);
	pFont.reset(TTF_OpenFont(path.c_str(), size), TTF_CloseFont);
	if (pFont == NULL)
		throw EngineException("SDL TTF Exception caught", __FILE__, "An error has been caught during TTF Font Opening.\nPlease check TTF file path.", __LINE__);
}


void GraphicsEngine::Font::DrawText(Maths::IVec2D pos, const std::string& text, GraphicsEngine::Color c)
{
	std::shared_ptr<SDL_Surface> textSurf(TTF_RenderText_Solid(pFont.get(), text.c_str(), c.c), SDL_FreeSurface);
	Sprite tSprite = Sprite(textSurf, textSurf->w, textSurf->h);
	Graphics::GetInstance().DrawSprite(Maths::IRect(pos.x, pos.y, textSurf->w, textSurf->h), tSprite, 10);
}
