#pragma once
#include "Graphics.h"

class Window {
friend Graphics::Graphics(Window&);
public:
	Window();
	~Window();

	SDL_Window* GetWindow();

private:
	SDL_Window* window;
};