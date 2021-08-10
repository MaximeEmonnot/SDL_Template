#pragma once
#include "SingletonMaker.h"
#include "Window.h"
#include "DColors.h"
#include "Sprite.h"
#include "Rect.h"
#include <map>
#include <functional>

namespace GraphicsEngine {
	class Graphics : public CoreSystem::SingletonMaker<GraphicsEngine::Graphics>{
	public:
		Graphics();
		~Graphics();
		void										BeginRender();
		void										EndRender();

		SDL_Renderer*								GetRenderer();

		// Draws a colored pixel at a given position. 
		void										DrawPixel(Maths::IVec2D pos, Color c, int priority = 0);
		void										DrawPixel(int x, int y, Color c, int priority = 0) {
			DrawPixel(Maths::IVec2D(x, y), c, priority);
		}
		void										DrawPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int priority = 0) {
			DrawPixel(x, y, Color(r, g, b, a), priority);
		}

		// Changes Background Color
		void										SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a, int priority = 0);
		void										SetBackgroundColor(Color c, int priority = 0) {
			SetBackgroundColor(c.c.r, c.c.g, c.c.b, c.c.a, priority);
		}

		// Draws the border of a colored rectangle at a given position.
		void										DrawRect(Maths::IRect rect, Color c, int priority = 0);
		void										DrawRect(int x, int y, int w, int h, Color c, int priority = 0) {
			DrawRect({ x, y, w, h }, c, priority);
		}
		void										DrawRect(Maths::IVec2D pos, int w, int h, Color c, int priority = 0) {
			DrawRect(Maths::IRect(pos, w, h), c, priority);
		}
		void										DrawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int priority = 0) {
			DrawRect(x, y, w, h, { r, g, b, a }, priority);
		}

		// Draws a full colored rectangle at a given position.
		void										DrawFilledRect(Maths::IRect rect, Color c, int priority = 0);
		void										DrawFilledRect(Maths::IVec2D pos, int w, int h, Color c, int priority = 0) {
			DrawFilledRect(Maths::IRect(pos, w, h), c, priority);
		}
		void										DrawFilledRect(int x, int y, int w, int h, Color c, int priority = 0) {
			DrawFilledRect({ x, y, w, h }, c, priority);
		}
		void										DrawFilledRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int priority = 0) {
			DrawFilledRect(x, y, w, h, { r, g, b, a }, priority);
		}

		// Draws a given sprite at a given position.
		void										DrawSprite(const Sprite& rSprite, int priority = 0) {
			DrawSprite(mScreenRect, rSprite, priority);
		}
		void										DrawSprite(Maths::IRect destRect, const Sprite& rSprite, int priority = 0) {
			DrawSprite(destRect, Maths::IRect(0, 0, rSprite.GetWidth(), rSprite.GetHeight()), rSprite, priority);
		}
		void										DrawSprite(Maths::IRect destRect, Maths::IRect srcRect, const Sprite& rSprite, int priority = 0);

		// Fades current screen
		void										FadeOutScreen(float percentage);
		void										FadeInScreen(float percentage);

		// Blend current screen to a defined color
		void										BlendScreenTo(Color c);

	private:
		SDL_Texture*								mpTextureTarget;
		SDL_Renderer*								mpRenderer;
		Maths::IRect								mScreenRect;
		std::multimap<int, std::function<void()>>	renderQueue;
	};
}