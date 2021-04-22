#include "Graphics.h"
#include "Window.h"

Graphics::Graphics(Window& window)
{
	renderer = SDL_CreateRenderer(window.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		throw Exception(__FILE__, __LINE__, "An error has been caught during SDL Renderer Creation.");
	}
}

Graphics::~Graphics()
{
	SDL_DestroyRenderer(renderer);
}

void Graphics::BeginRender()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void Graphics::EndRender()
{
	SDL_RenderPresent(renderer);
}

SDL_Renderer* Graphics::GetRenderer()
{
	return renderer;
}

void Graphics::DrawPixel(IVec2D pos, SDL_Color c)
{
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawPoint(renderer, pos.x, pos.y);
}

void Graphics::SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderFillRect(renderer, &screenRect);
}

void Graphics::DrawRect(SDL_Rect rect, SDL_Color c)
{
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawRect(renderer, &rect);
}

void Graphics::DrawFilledRect(SDL_Rect rect, SDL_Color c)
{
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderFillRect(renderer, &rect);
}

void Graphics::DrawSprite(SDL_Rect destRect, SDL_Rect srcRect, const Sprite& s)
{
	SDL_RenderCopy(renderer, s.GetTexture(), &srcRect, &destRect);
}
