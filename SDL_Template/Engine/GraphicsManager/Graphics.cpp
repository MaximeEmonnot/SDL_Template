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
	auto func = [renderer = mpRenderer, red = c.c.r, green = c.c.g, blue = c.c.b, alpha = c.c.a, x = pos.x, y = pos.y] {
		SDL_SetRenderDrawColor(renderer.get(), red, green, blue, alpha);
		SDL_RenderDrawPoint(renderer.get(), x, y);
	};
	renderQueue.insert(std::pair<int, std::function<void()>>(priority, func));
}

void GraphicsEngine::Graphics::SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a, int priority)
{
	auto func = [renderer = mpRenderer, red = r, green = g, blue = b, alpha = a, rect = mScreenRect.rect] {
		SDL_SetRenderDrawColor(renderer.get(), red, green, blue, alpha);
		SDL_RenderFillRect(renderer.get(), &rect);
	};
	renderQueue.insert(std::pair<int, std::function<void()>>(priority, func));
}

void GraphicsEngine::Graphics::DrawRect(Maths::IRect rect, Color c, int priority)
{
	auto func = [renderer = mpRenderer, red = c.c.r, green = c.c.g, blue = c.c.b, alpha = c.c.a, rect = rect.rect] {
		SDL_SetRenderDrawColor(renderer.get(), red, green, blue, alpha);
		SDL_RenderDrawRect(renderer.get(), &rect);
	};
	renderQueue.insert(std::pair<int, std::function<void()>>(priority, func));
}

void GraphicsEngine::Graphics::DrawFilledRect(Maths::IRect rect, Color c, int priority)
{
	auto func = [renderer = mpRenderer, red = c.c.r, green = c.c.g, blue = c.c.b, alpha = c.c.a, rect = rect.rect]{
		SDL_SetRenderDrawColor(renderer.get(), red, green, blue, alpha);
		SDL_RenderFillRect(renderer.get(), &rect);
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
 
void GraphicsEngine::Graphics::FadeOutScreen(float percentage)
{
	Uint8 red, green, blue;
	SDL_GetTextureColorMod(mpTextureTarget.get(), &red, &green, &blue);
	SDL_SetTextureColorMod(mpTextureTarget.get(), Uint8(percentage * red), Uint8(percentage * green), Uint8(percentage * blue));
}

void GraphicsEngine::Graphics::FadeInScreen(float percentage)
{
	Uint8 red, green, blue;
	SDL_GetTextureColorMod(mpTextureTarget.get(), &red, &green, &blue);
	SDL_SetTextureColorMod(mpTextureTarget.get(), Uint8((1.0f - percentage) * red), Uint8((1.0f - percentage) * green), Uint8((1.0f - percentage) * blue));
}

void GraphicsEngine::Graphics::BlendScreenTo(Color c)
{
	SDL_SetTextureColorMod(mpTextureTarget.get(), c.c.r, c.c.g, c.c.b);
}
