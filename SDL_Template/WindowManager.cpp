#include "WindowManager.h"

Window::Window()
	:
	running(true)
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Template SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Graphics::width, Graphics::height, SDL_WINDOW_OPENGL);
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
		case SDL_WINDOWEVENT:
		{
			switch (event.window.event) {
			case SDL_WINDOWEVENT_FOCUS_LOST:
				kbd.Flush();
				break;
			default:
				break;
			}
		}
			break;
			// BEGIN KEYBOARD EVENTS
		case SDL_KEYDOWN:
			kbd.OnKeyPressed(event.key.keysym.sym);
			kbd.OnChar(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			kbd.OnKeyReleased(event.key.keysym.sym);
			break;
			// END KEYBOARD EVENTS

			// BEGIN MOUSE EVENTS
		case SDL_MOUSEBUTTONDOWN:
		{
			if (event.button.button == SDL_BUTTON_LEFT)
				mouse.OnLeftPressed();
			else if (event.button.button == SDL_BUTTON_RIGHT)
				mouse.OnRightPressed();
		}
			break;
		case SDL_MOUSEBUTTONUP:
		{
			if (event.button.button == SDL_BUTTON_LEFT)
				mouse.OnLeftReleased();
			else if (event.button.button == SDL_BUTTON_RIGHT)
				mouse.OnRightReleased();
		}
			break;
		case SDL_MOUSEWHEEL:
		{
			if (event.wheel.y > 0)
				mouse.OnWheelUp();
			else if (event.wheel.y < 0)
				mouse.OnWheelDown();
		}
			break;
		case SDL_MOUSEMOTION: 
			mouse.OnMouseMove((int)event.button.x, (int)event.button.y);
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
