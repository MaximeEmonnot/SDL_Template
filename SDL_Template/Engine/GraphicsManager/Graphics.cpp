#include "Graphics.h"

GraphicsEngine::Graphics::Graphics(Window& window)
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

void GraphicsEngine::Graphics::DrawPixel(IVec2D pos, SDL_Color c)
{
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawPoint(renderer, pos.x, pos.y);
}

void GraphicsEngine::Graphics::SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderFillRect(renderer, &screenRect);
}

void GraphicsEngine::Graphics::DrawRect(SDL_Rect rect, SDL_Color c)
{
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawRect(renderer, &rect);
}

void GraphicsEngine::Graphics::DrawFilledRect(SDL_Rect rect, SDL_Color c)
{
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderFillRect(renderer, &rect);
}

void GraphicsEngine::Graphics::DrawSprite(SDL_Rect destRect, SDL_Rect srcRect, const Sprite& s)
{
	SDL_RenderCopy(renderer, s.GetTexture(), &srcRect, &destRect);
}
