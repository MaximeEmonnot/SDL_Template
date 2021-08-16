#include "Graphics.h"

GraphicsEngine::Graphics::Graphics()
{
	std::shared_ptr<CoreSystem::Window> window = CoreSystem::Window::GetInstance();
	mpRenderer.reset(SDL_CreateRenderer(window->pGetWindow().get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), SDL_DestroyRenderer);
	if (mpRenderer == NULL) {
		throw EngineException("SDL Graphics Exception caught", __FILE__, "An error has been caught during SDL Renderer Creation.", __LINE__);
	}
	mScreenRect = window->GetScreenRect();
	mpTextureTarget.reset(SDL_CreateTexture(mpRenderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, mScreenRect.rect.w, mScreenRect.rect.h), SDL_DestroyTexture);
	if (mpTextureTarget == NULL) {
		throw EngineException("SDL Graphics Exception caught", __FILE__, "An error has been caught during SDL Texture Target Creation.\nMore informations :" + std::string(SDL_GetError()), __LINE__);
	}
}

GraphicsEngine::Graphics::~Graphics()
{
}

void GraphicsEngine::Graphics::BeginRender()
{
	renderQueue.clear();
	SDL_SetRenderTarget(mpRenderer.get(), mpTextureTarget.get());
	SDL_SetTextureColorMod(mpTextureTarget.get(), 255, 255, 255);
	SDL_SetRenderDrawColor(mpRenderer.get(), 0, 0, 0, 255);
	SDL_RenderClear(mpRenderer.get());
}

void GraphicsEngine::Graphics::EndRender()
{
	//Render Texture Target depending on priority
	for (auto& entry : renderQueue) {
		entry.second();
	}
	//Actual Draw
	SDL_SetRenderTarget(mpRenderer.get(), NULL);
	SDL_RenderCopy(mpRenderer.get(), mpTextureTarget.get(), NULL, NULL);
	SDL_RenderPresent(mpRenderer.get());
}

std::shared_ptr<SDL_Renderer> GraphicsEngine::Graphics::GetRenderer()
{
	return mpRenderer;
}

void GraphicsEngine::Graphics::DrawPixel(Maths::IVec2D pos, Color c, int priority)
{
	auto func = [=] {
		SDL_SetRenderDrawColor(mpRenderer.get(), c.c.r, c.c.g, c.c.b, c.c.a);
		SDL_RenderDrawPoint(mpRenderer.get(), pos.x, pos.y);
	};
	renderQueue.insert(std::pair<int, std::function<void()>>(priority, func));
}

void GraphicsEngine::Graphics::SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a, int priority)
{
	auto func = [=] {
		SDL_SetRenderDrawColor(mpRenderer.get(), r, g, b, a);
		SDL_RenderFillRect(mpRenderer.get(), &mScreenRect.rect);
	};
	renderQueue.insert(std::pair<int, std::function<void()>>(priority, func));
}

void GraphicsEngine::Graphics::DrawRect(Maths::IRect rect, Color c, int priority)
{
	auto func = [=] {
		SDL_SetRenderDrawColor(mpRenderer.get(), c.c.r, c.c.g, c.c.b, c.c.a);
		SDL_RenderDrawRect(mpRenderer.get(), &rect.rect);
	};
	renderQueue.insert(std::pair<int, std::function<void()>>(priority, func));
}

void GraphicsEngine::Graphics::DrawFilledRect(Maths::IRect rect, Color c, int priority)
{
	auto func = [=]{
		SDL_SetRenderDrawColor(mpRenderer.get(), c.c.r, c.c.g, c.c.b, c.c.a);
		SDL_RenderFillRect(mpRenderer.get(), &rect.rect);
	};
	renderQueue.insert(std::pair<int, std::function<void()>>(priority, func));
}

void GraphicsEngine::Graphics::DrawSprite(Maths::IRect destRect, Maths::IRect srcRect, const Sprite& s, int priority)
{
	auto func = [=]{
		SDL_RenderCopy(mpRenderer.get(), s.GetTexture().get(), &srcRect.rect, &destRect.rect);
	};
	renderQueue.insert(std::pair<int, std::function<void()>>(priority, func));
}
 
void GraphicsEngine::Graphics::DrawInvertedSprite(Maths::IRect destRect, Maths::IRect srcRect, const Sprite& rSprite, int priority)
{
	auto func = [=] {
		Sprite s = rSprite;
		s.BlendColor(Color(95, 153, 153, 255));
		SDL_RenderCopyEx(mpRenderer.get(), s.GetTexture().get(), &srcRect.rect, &destRect.rect, 0, NULL, SDL_FLIP_VERTICAL);
		s.BlendColor(WHITE);
	};
	renderQueue.insert(std::pair<int, std::function<void()>>(priority, func));
}

void GraphicsEngine::Graphics::FadeOutScreen(float percentage)
{
	Uint8 red, green, blue;
	SDL_GetTextureColorMod(mpTextureTarget.get(), &red, &green, &blue);
	SDL_SetTextureColorMod(mpTextureTarget.get(), static_cast<Uint8>(percentage * red), static_cast<Uint8>(percentage * green), static_cast<Uint8>(percentage * blue));
}

void GraphicsEngine::Graphics::FadeInScreen(float percentage)
{
	Uint8 red, green, blue;
	SDL_GetTextureColorMod(mpTextureTarget.get(), &red, &green, &blue);
	SDL_SetTextureColorMod(mpTextureTarget.get(), static_cast<Uint8>((1.0f - percentage) * red), static_cast<Uint8>((1.0f - percentage) * green), static_cast<Uint8>((1.0f - percentage) * blue));
}

void GraphicsEngine::Graphics::BlendScreenTo(Color c)
{
	SDL_SetTextureColorMod(mpTextureTarget.get(), c.c.r, c.c.g, c.c.b);
}
