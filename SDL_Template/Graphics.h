#pragma once
#include <SDL.h>

class Graphics {
public:
	Graphics(class Window& window);
	~Graphics();
	void BeginRender();
	void EndRender();

	void RenderColorTest(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
	void RenderDrawRectTest(SDL_Rect rect, SDL_Color c);
	void RenderDrawRectTest(int x, int y, int w, int h, SDL_Color c) {
		RenderDrawRectTest({ x, y, w, h }, c);
	}

private:
	SDL_Renderer* renderer;

public:
	static constexpr int width = 800;
	static constexpr int height = 600;
};