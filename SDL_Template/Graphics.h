#pragma once
#include <SDL.h>
#include "Surface.h"
#include "Vec2D.h"

class Graphics {
public:
	Graphics(class Window& window);
	~Graphics();
	void						BeginRender();
	void						EndRender();

	SDL_Renderer*				GetRenderer();

	void						DrawPixel(IVec2D pos, SDL_Color c);
	void						DrawPixel(int x, int y, SDL_Color c) {
		DrawPixel(IVec2D(x, y), c);
	}
	void						DrawPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		DrawPixel(x, y, { r, g, b, a });
	}

	void						SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void						SetBackgroundColor(SDL_Color c) {
		SetBackgroundColor(c.r, c.g, c.b, c.a);
	}

	void						DrawRect(SDL_Rect rect, SDL_Color c);
	void						DrawRect(int x, int y, int w, int h, SDL_Color c) {
		DrawRect({ x, y, w, h }, c);
	}
	void						DrawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		DrawRect(x, y, w, h, { r, g, b, a });
	}

	void						DrawFilledRect(SDL_Rect rect, SDL_Color c);
	void						DrawFilledRect(int x, int y, int w, int h, SDL_Color c) {
		DrawFilledRect({ x, y, w, h }, c);
	}
	void						DrawFilledRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		DrawFilledRect(x, y, w, h, { r, g, b, a });
	}

	void						DrawSprite(SDL_Rect destRect, const Surface& s) {
		DrawSprite(destRect, { 0, 0, s.GetWidth(), s.GetHeight() }, s);
	}
	void						DrawSprite(SDL_Rect destRect, SDL_Rect srcRect, const Surface& s);

private:
	SDL_Renderer*				renderer;

public:
	static constexpr int		width = 800;
	static constexpr int		height = 600;
	static constexpr SDL_Rect	screenRect = { 0, 0, width, height };
};