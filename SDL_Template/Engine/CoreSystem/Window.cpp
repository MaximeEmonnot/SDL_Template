#include "Window.h"
#include "Graphics.h"

CoreSystem::Window::Window()
	:
	mbIsMinimized(false)
{
	mEvent.type = SDL_FIRSTEVENT;
	mbIsRunning = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw EngineException("SDL Window Exception caught", __FILE__, "An error has been caught during SDL Initialisation.\nMore informations: " + std::string(SDL_GetError()), __LINE__);
	}
	mpWindow.reset(SDL_CreateWindow("Template SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth, mHeight, SDL_WINDOW_OPENGL), SDL_DestroyWindow);
	if (mpWindow == NULL) {
		throw EngineException("SDL Window Exception caught", __FILE__, "An error has been caught during Window Creation.\nMore informations: " + std::string(SDL_GetError()), __LINE__);
	}
	SDL_Surface* surf = IMG_Load("Images\\icon.png");
	if (surf == nullptr) {
		throw EngineException("SDL Window Exception caught", __FILE__, "An error has been caught during Icon loading.\nPlease check filename (MUST be \"icon.png\").\nMore informations: " + std::string(SDL_GetError()), __LINE__);
	}
	SDL_SetWindowIcon(mpWindow.get(), surf);
	SDL_ShowCursor(1);
}

CoreSystem::Window::~Window()
{
	SDL_Quit();
}

bool CoreSystem::Window::ListensToEvents() noexcept
{
	while (SDL_PollEvent(&mEvent)) {
		switch (mEvent.type) {
		case SDL_QUIT:
			mbIsRunning = false;
			break;
		case SDL_WINDOWEVENT:
		{
			switch (mEvent.window.event) {
			case SDL_WINDOWEVENT_FOCUS_LOST:
				Keyboard::GetInstance().Flush();
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				mbIsMinimized = true;
				while (SDL_WaitEvent(&mEvent)) {
					if (mEvent.window.event == SDL_WINDOWEVENT_RESTORED) {
						mbIsMinimized = false;
						break;
					}
				}
				break;
			default:
				break;
			}
		}
			break;
			// *************** BEGIN KEYBOARD EVENTS *************** //
		case SDL_KEYDOWN:
			Keyboard::GetInstance().OnKeyPressed(mEvent.key.keysym.scancode);
			Keyboard::GetInstance().OnChar(mEvent.key.keysym.sym);
			break;
		case SDL_KEYUP:
			Keyboard::GetInstance().OnKeyReleased(mEvent.key.keysym.scancode);
			break;
			// ***************** END KEYBOARD EVENTS *************** //

			// ***************** BEGIN MOUSE EVENTS **************** //
		case SDL_MOUSEBUTTONDOWN:
		{
			if (mEvent.button.button == SDL_BUTTON_LEFT)
				Mouse::GetInstance().OnLeftPressed();
			else if (mEvent.button.button == SDL_BUTTON_RIGHT)
				Mouse::GetInstance().OnRightPressed();
		}
			break;
		case SDL_MOUSEBUTTONUP:
		{
			if (mEvent.button.button == SDL_BUTTON_LEFT)
				Mouse::GetInstance().OnLeftReleased();
			else if (mEvent.button.button == SDL_BUTTON_RIGHT)
				Mouse::GetInstance().OnRightReleased();
		}
			break;
		case SDL_MOUSEWHEEL:
		{
			if (mEvent.wheel.y > 0)
				Mouse::GetInstance().OnWheelUp();
			else if (mEvent.wheel.y < 0)
				Mouse::GetInstance().OnWheelDown();
		}
			break;
		case SDL_MOUSEMOTION: 
			Mouse::GetInstance().OnMouseMove(static_cast<int>(mEvent.button.x), static_cast<int>(mEvent.button.y));
			break;
			// ****************** END MOUSE EVENTS ***************** //

		default:
			break;
		}
	}
	return mbIsRunning;
}

void CoreSystem::Window::ExitGame()
{
	mbIsRunning = false;
}

std::shared_ptr<SDL_Window> CoreSystem::Window::pGetWindow()
{
	return mpWindow;
}

const int CoreSystem::Window::GetWidth() const
{
	return mWidth;
}

const int CoreSystem::Window::GetHeight() const
{
	return mHeight;
}

const Maths::IRect CoreSystem::Window::GetScreenRect() const
{
	return mScreenRect;
}

bool CoreSystem::Window::IsMinimized() const
{
	return mbIsMinimized;
}
