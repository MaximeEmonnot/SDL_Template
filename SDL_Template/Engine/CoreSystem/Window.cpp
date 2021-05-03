#include "Window.h"
#include "Graphics.h"

Window::Window()
{
	event.type = SDL_FIRSTEVENT;
	running = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw SDLException("SDL Window Exception caught", __FILE__, __LINE__, "An error has been caught during SDL Initialisation.");
	}
	window = SDL_CreateWindow("Template SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if (window == NULL) {
		throw SDLException("SDL Window Exception caught", __FILE__, __LINE__, "An error has been caught during Window Creation.");
	}
	SDL_Surface* surf = IMG_Load("Images\\icon.png");
	if (surf == nullptr) {
		throw SDLException("SDL Window Exception caught", __FILE__, __LINE__, "An error has been caught during Icon loading.\nPlease check filename (MUST be \"icon.png\").");
	}
	SDL_SetWindowIcon(window, surf);
	SDL_ShowCursor(1);
}

Window::~Window()
{
	kbd.Flush();
	mouse.Flush();
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool Window::EventListener() noexcept
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
			// *************** BEGIN KEYBOARD EVENTS *************** //
		case SDL_KEYDOWN:
			kbd.OnKeyPressed(event.key.keysym.scancode);
			kbd.OnChar(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			kbd.OnKeyReleased(event.key.keysym.scancode);
			break;
			// ***************** END KEYBOARD EVENTS *************** //

			// ***************** BEGIN MOUSE EVENTS **************** //
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
			// ****************** END MOUSE EVENTS ***************** //

		default:
			break;
		}
	}
	return running;
}

SDL_Window* Window::GetWindow()
{
	return window;
}

const int Window::GetWidth() const
{
	return width;
}

const int Window::GetHeight() const
{
	return height;
}

const SDL_Rect Window::GetScreenRect() const
{
	return screenRect;
}
