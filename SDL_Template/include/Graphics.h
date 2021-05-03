#pragma once
#include "Window.h"
#include "DColors.h"
#include "Sprite.h"
#include "Vec2D.h"

namespace GraphicsEngine {
	class Graphics {
	public:
		Graphics(CoreSystem::Window& window);
		~Graphics();
		void						BeginRender();
		void						EndRender();

		SDL_Renderer* GetRenderer();

		// Draws a colored pixel at a given position. 
		void						DrawPixel(Maths::IVec2D pos, SDL_Color c);
		void						DrawPixel(int x, int y, SDL_Color c) {
			DrawPixel(Maths::IVec2D(x, y), c);
		}
		void						DrawPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
			DrawPixel(x, y, { r, g, b, a });
		}

		// Changes Background Color
		void						SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		void						SetBackgroundColor(SDL_Color c) {
			SetBackgroundColor(c.r, c.g, c.b, c.a);
		}

		// Draws the border of a colored rectangle at a given position.
		void						DrawRect(SDL_Rect rect, SDL_Color c);
		void						DrawRect(int x, int y, int w, int h, SDL_Color c) {
			DrawRect({ x, y, w, h }, c);
		}
		void						DrawRect(Maths::IVec2D pos, int w, int h, SDL_Color c) {
			DrawRect(Maths::VecToRect(pos, w, h), c);
		}
		void						DrawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
			DrawRect(x, y, w, h, { r, g, b, a });
		}

		// Draws a full colored rectangle at a given position.
		void						DrawFilledRect(SDL_Rect rect, SDL_Color c);
		void						DrawFilledRect(Maths::IVec2D pos, int w, int h, SDL_Color c) {
			DrawFilledRect(Maths::VecToRect(pos, w, h), c);
		}
		void						DrawFilledRect(int x, int y, int w, int h, SDL_Color c) {
			DrawFilledRect({ x, y, w, h }, c);
		}
		void						DrawFilledRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
			DrawFilledRect(x, y, w, h, { r, g, b, a });
		}

		// Draws a given sprite at a given position.
		void						DrawSprite(SDL_Rect destRect, const Sprite& s) {
			DrawSprite(destRect, { 0, 0, s.GetWidth(), s.GetHeight() }, s);
		}
		void						DrawSprite(SDL_Rect destRect, SDL_Rect srcRect, const Sprite& s);

	private:
		SDL_Renderer* renderer;
		const SDL_Rect              screenRect;
	};
}