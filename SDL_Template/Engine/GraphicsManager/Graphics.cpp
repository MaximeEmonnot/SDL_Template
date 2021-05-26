#include "Graphics.h"

GraphicsEngine::Graphics::Graphics(CoreSystem::Window& window)
	:
	screenRect(window.GetScreenRect())
{
	renderer = SDL_CreateRenderer(window.GetWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		throw SDLException("SDL Graphics Exception caught", __FILE__, __LINE__, "An error has been caught during SDL Renderer Creation.");
	}
}

GraphicsEngine::Graphics::~Graphics()
{
	SDL_DestroyRenderer(renderer);
}

void GraphicsEngine::Graphics::BeginRender()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void GraphicsEngine::Graphics::EndRender()
{
	SDL_RenderPresent(renderer);
}

SDL_Renderer* GraphicsEngine::Graphics::GetRenderer()
{
	return renderer;
}

void GraphicsEngine::Graphics::DrawPixel(Maths::IVec2D pos, Color c)
{
	SDL_SetRenderDrawColor(renderer, c.c.r, c.c.g, c.c.b, c.c.a);
	SDL_RenderDrawPoint(renderer, pos.x, pos.y);
}

void GraphicsEngine::Graphics::SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderFillRect(renderer, &screenRect);
}

void GraphicsEngine::Graphics::DrawRect(Maths::IRect rect, Color c)
{
	SDL_SetRenderDrawColor(renderer, c.c.r, c.c.g, c.c.b, c.c.a);
	SDL_RenderDrawRect(renderer, &rect.rect);
}

void GraphicsEngine::Graphics::DrawFilledRect(Maths::IRect rect, Color c)
{
	SDL_SetRenderDrawColor(renderer, c.c.r, c.c.g, c.c.b, c.c.a);
	SDL_RenderFillRect(renderer, &rect.rect);
}

void GraphicsEngine::Graphics::DrawSprite(Maths::IRect destRect, Maths::IRect srcRect, const Sprite& s)
{
	SDL_RenderCopy(renderer, s.GetTexture(), &srcRect.rect, &destRect.rect);
}
