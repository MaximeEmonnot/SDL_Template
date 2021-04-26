#include "Window.h"

Window::Window()
{
	event.type = SDL_FIRSTEVENT;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw Exception(__FILE__, __LINE__, "An error has been caught during SDL Initialisation.");
	}
	window = SDL_CreateWindow("Template SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Graphics::width, Graphics::height, SDL_WINDOW_OPENGL);
	if (window == NULL) {
		throw Exception(__FILE__, __LINE__, "An error has been caught during Window Creation.");
	}
	SDL_Surface* surf = IMG_Load("icon.png");
	if (surf == nullptr) {
		throw Exception(__FILE__, __LINE__, "An error has been caught during Icon loading.\nPlease check filename (MUST be \"icon.png\").");
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
			return false;
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
	return true;
}