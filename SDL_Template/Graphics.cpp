#include "Graphics.h"
#include "WindowManager.h"

Graphics::Graphics(Window& window)
{
	renderer = SDL_CreateRenderer(window.GetWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

Graphics::~Graphics()
{
	SDL_DestroyRenderer(renderer);
}

void Graphics::BeginRender()
{
	SDL_RenderClear(renderer);
}

void Graphics::EndRender()
{
	SDL_RenderPresent(renderer);
}

void Graphics::RenderColorTest(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
	SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
}

void Graphics::RenderDrawRectTest(SDL_Rect rect, SDL_Color c)
{
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawRect(renderer, &rect);
}
