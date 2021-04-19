#include "WindowManager.h"

Window::Window()
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Test SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Graphics::width, Graphics::height, SDL_WINDOW_OPENGL);
}

Window::~Window()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

SDL_Window* Window::GetWindow()
{
	return window;
}
