#include "Graphics.h"

GraphicsEngine::Graphics::Graphics()
{
	std::shared_ptr<CoreSystem::Window> window = CoreSystem::Window::GetInstance();
	mpRenderer = SDL_CreateRenderer(window->pGetWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (mpRenderer == NULL) {
		throw EngineException("SDL Graphics Exception caught", __FILE__, "An error has been caught during SDL Renderer Creation.", __LINE__);
	}
	mScreenRect = window->GetScreenRect();
	mpTextureTarget = SDL_CreateTexture(mpRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, mScreenRect.rect.w, mScreenRect.rect.h);
	if (mpTextureTarget == NULL) {
		throw EngineException("SDL Graphics Exception caught", __FILE__, "An error has been caught during SDL Texture Target Creation.\nMore informations :" + std::string(SDL_GetError()), __LINE__);
	}
}

GraphicsEngine::Graphics::~Graphics()
{
	SDL_DestroyTexture(mpTextureTarget);
	SDL_DestroyRenderer(mpRenderer);
}

void GraphicsEngine::Graphics::BeginRender()
{
	SDL_SetRenderTarget(mpRenderer, mpTextureTarget);
	SDL_SetTextureColorMod(mpTextureTarget, 255, 255, 255);
	SDL_SetRenderDrawColor(mpRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mpRenderer);
}

void GraphicsEngine::Graphics::EndRender()
{
	SDL_SetRenderTarget(mpRenderer, NULL);
	SDL_RenderCopy(mpRenderer, mpTextureTarget, NULL, NULL);
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

void GraphicsEngine::Graphics::FadeOutScreen(float percentage)
{
	Uint8 red, green, blue;
	SDL_GetTextureColorMod(mpTextureTarget, &red, &green, &blue);
	SDL_SetTextureColorMod(mpTextureTarget, Uint8(percentage * red), Uint8(percentage * green), Uint8(percentage * blue));
}

void GraphicsEngine::Graphics::FadeInScreen(float percentage)
{
	Uint8 red, green, blue;
	SDL_GetTextureColorMod(mpTextureTarget, &red, &green, &blue);
	SDL_SetTextureColorMod(mpTextureTarget, Uint8((1.0f - percentage) * red), Uint8((1.0f - percentage) * green), Uint8((1.0f - percentage) * blue));
}

void GraphicsEngine::Graphics::BlendScreenTo(Color c)
{
	SDL_SetTextureColorMod(mpTextureTarget, c.c.r, c.c.g, c.c.b);
}
