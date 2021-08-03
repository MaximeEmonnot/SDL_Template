#pragma once
#include "SingletonMaker.h"
#include "Window.h"
#include "DColors.h"
#include "Sprite.h"
#include "Rect.h"

namespace GraphicsEngine {
	class Graphics : public CoreSystem::SingletonMaker<GraphicsEngine::Graphics>{
	public:
		Graphics();
		~Graphics();
		void						BeginRender();
		void						EndRender();

		SDL_Renderer* GetRenderer();

		// Draws a colored pixel at a given position. 
		void						DrawPixel(Maths::IVec2D pos, Color c);
		void						DrawPixel(int x, int y, Color c) {
			DrawPixel(Maths::IVec2D(x, y), c);
		}
		void						DrawPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
			DrawPixel(x, y, Color(r, g, b, a));
		}

		// Changes Background Color
		void						SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		void						SetBackgroundColor(Color c) {
			SetBackgroundColor(c.c.r, c.c.g, c.c.b, c.c.a);
		}

		// Draws the border of a colored rectangle at a given position.
		void						DrawRect(Maths::IRect rect, Color c);
		void						DrawRect(int x, int y, int w, int h, Color c) {
			DrawRect({ x, y, w, h }, c);
		}
		void						DrawRect(Maths::IVec2D pos, int w, int h, Color c) {
			DrawRect(Maths::IRect(pos, w, h), c);
		}
		void						DrawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
			DrawRect(x, y, w, h, { r, g, b, a });
		}

		// Draws a full colored rectangle at a given position.
		void						DrawFilledRect(Maths::IRect rect, Color c);
		void						DrawFilledRect(Maths::IVec2D pos, int w, int h, Color c) {
			DrawFilledRect(Maths::IRect(pos, w, h), c);
		}
		void						DrawFilledRect(int x, int y, int w, int h, Color c) {
			DrawFilledRect({ x, y, w, h }, c);
		}
		void						DrawFilledRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
			DrawFilledRect(x, y, w, h, { r, g, b, a });
		}

		// Draws a given sprite at a given position.
		void						DrawSprite(const Sprite& rSprite) {
			DrawSprite(mScreenRect, rSprite);
		}
		void						DrawSprite(Maths::IRect destRect, const Sprite& rSprite) {
			DrawSprite(destRect, Maths::IRect(0, 0, rSprite.GetWidth(), rSprite.GetHeight()), rSprite);
		}
		void						DrawSprite(Maths::IRect destRect, Maths::IRect srcRect, const Sprite& rSprite);

		// Fades current screen
		void                        FadeOutScreen(float percentage);
		void                        FadeInScreen(float percentage);

		// Blend current screen to a defined color
		void                        BlendScreenTo(Color c);

	private:
		SDL_Texture*				mpTextureTarget;
		SDL_Renderer*				mpRenderer;
		Maths::IRect				mScreenRect;
	};
}