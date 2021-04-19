#include "WindowManager.h"

Window::Window()
	:
	mouse(*this),
	kbd(),
	running(true)
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Test SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Graphics::width, Graphics::height, SDL_WINDOW_OPENGL);
}

Window::~Window()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Window::EventListener()
{
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			break;
		case SDL_KEYUP:
			break;
			// BEGIN MOUSE EVENTS
		case SDL_MOUSEBUTTONDOWN:
		{
			if (event.button.button == SDL_BUTTON_LEFT)
				mouse.SetType(Mouse::Type::LPress);
			else if (event.button.button == SDL_BUTTON_RIGHT)
				mouse.SetType(Mouse::Type::RPress);
		}
			break;
		case SDL_MOUSEBUTTONUP:
		{
			if (event.button.button == SDL_BUTTON_LEFT)
				mouse.SetType(Mouse::Type::LRelease);
			else if (event.button.button == SDL_BUTTON_RIGHT)
				mouse.SetType(Mouse::Type::RRelease);
		}
			break;
		case SDL_MOUSEWHEEL:
		{
			if (event.wheel.y > 0)
				mouse.SetType(Mouse::Type::WheelUp);
			else if (event.wheel.y < 0)
				mouse.SetType(Mouse::Type::WheelDown);
		}
			break;
		case SDL_MOUSEMOTION: 
			mouse.SetType(Mouse::Type::Move);
			break;

			// END MOUSE EVENTS
		default:
			break;
		}
	}
}

SDL_Window* Window::GetWindow()
{
	return window;
}

SDL_Event& Window::GetEvent()
{
	return event;
}

bool Window::IsRunning() const
{
	return running;
}
