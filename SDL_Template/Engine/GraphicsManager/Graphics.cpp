#include "Graphics.h"

GraphicsEngine::Graphics::Graphics()
{
	std::shared_ptr<CoreSystem::Window> window = CoreSystem::Window::GetInstance();
	mpRenderer = SDL_CreateRenderer(window->pGetWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	mScreenRect = window->GetScreenRect();
	if (mpRenderer == NULL) {
		throw EngineException("SDL Graphics Exception caught", __FILE__, "An error has been caught during SDL Renderer Creation.", __LINE__);
	}
}

GraphicsEngine::Graphics::~Graphics()
{
	SDL_DestroyRenderer(mpRenderer);
}

void GraphicsEngine::Graphics::BeginRender()
{
	SDL_SetRenderDrawColor(mpRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mpRenderer);
}

void GraphicsEngine::Graphics::EndRender()
{
	SDL_RenderPresent(mpRenderer);
}

SDL_Renderer* GraphicsEngine::Graphics::GetRenderer()
{
	return mpRenderer;
}

void GraphicsEngine::Graphics::DrawPixel(Maths::IVec2D pos, Color c)
{
	SDL_SetRenderDrawColor(mpRenderer, c.c.r, c.c.g, c.c.b, c.c.a);
	SDL_RenderDrawPoint(mpRenderer, pos.x, pos.y);
}

void GraphicsEngine::Graphics::SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(mpRenderer, r, g, b, a);
	SDL_RenderFillRect(mpRenderer, &mScreenRect.rect);
}

void GraphicsEngine::Graphics::DrawRect(Maths::IRect rect, Color c)
{
	SDL_SetRenderDrawColor(mpRenderer, c.c.r, c.c.g, c.c.b, c.c.a);
	SDL_RenderDrawRect(mpRenderer, &rect.rect);
}

void GraphicsEngine::Graphics::DrawFilledRect(Maths::IRect rect, Color c)
{
	SDL_SetRenderDrawColor(mpRenderer, c.c.r, c.c.g, c.c.b, c.c.a);
	SDL_RenderFillRect(mpRenderer, &rect.rect);
}

void GraphicsEngine::Graphics::DrawSprite(Maths::IRect destRect, Maths::IRect srcRect, const Sprite& s)
{
	SDL_RenderCopy(mpRenderer, s.GetTexture(), &srcRect.rect, &destRect.rect);
}
