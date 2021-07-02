#include "Window.h"
#include "Graphics.h"

CoreSystem::Window::Window()
	:
	pTimer(Timer::GetInstance()),
	pKbd(Keyboard::GetInstance()),
	pMouse(Mouse::GetInstance()),
	pThreadPool(ThreadPool::GetInstance(30)),
	pSoundSystem(SoundEngine::SoundSystem::GetInstance())
{
	mEvent.type = SDL_FIRSTEVENT;
	mbIsRunning = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw EngineException("SDL Window Exception caught", __FILE__, "An error has been caught during SDL Initialisation.", __LINE__);
	}
	mpWindow = SDL_CreateWindow("Template SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth, mHeight, SDL_WINDOW_OPENGL);
	if (mpWindow == NULL) {
		throw EngineException("SDL Window Exception caught", __FILE__, "An error has been caught during Window Creation.", __LINE__);
	}
	SDL_Surface* surf = IMG_Load("Images\\icon.png");
	if (surf == nullptr) {
		throw EngineException("SDL Window Exception caught", __FILE__, "An error has been caught during Icon loading.\nPlease check filename (MUST be \"icon.png\").", __LINE__);
	}
	SDL_SetWindowIcon(mpWindow, surf);
	SDL_ShowCursor(1);
}

CoreSystem::Window::~Window()
{
	pSoundSystem->Kill();
	pThreadPool->Kill();
	pMouse->Kill();
	pKbd->Kill();
	pTimer->Kill();
	SDL_DestroyWindow(mpWindow);
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
				pKbd->Flush();
				break;
			default:
				break;
			}
		}
			break;
			// *************** BEGIN KEYBOARD EVENTS *************** //
		case SDL_KEYDOWN:
			pKbd->OnKeyPressed(mEvent.key.keysym.scancode);
			pKbd->OnChar(mEvent.key.keysym.sym);
			break;
		case SDL_KEYUP:
			pKbd->OnKeyReleased(mEvent.key.keysym.scancode);
			break;
			// ***************** END KEYBOARD EVENTS *************** //

			// ***************** BEGIN MOUSE EVENTS **************** //
		case SDL_MOUSEBUTTONDOWN:
		{
			if (mEvent.button.button == SDL_BUTTON_LEFT)
				pMouse->OnLeftPressed();
			else if (mEvent.button.button == SDL_BUTTON_RIGHT)
				pMouse->OnRightPressed();
		}
			break;
		case SDL_MOUSEBUTTONUP:
		{
			if (mEvent.button.button == SDL_BUTTON_LEFT)
				pMouse->OnLeftReleased();
			else if (mEvent.button.button == SDL_BUTTON_RIGHT)
				pMouse->OnRightReleased();
		}
			break;
		case SDL_MOUSEWHEEL:
		{
			if (mEvent.wheel.y > 0)
				pMouse->OnWheelUp();
			else if (mEvent.wheel.y < 0)
				pMouse->OnWheelDown();
		}
			break;
		case SDL_MOUSEMOTION: 
			pMouse->OnMouseMove((int)mEvent.button.x, (int)mEvent.button.y);
			break;
			// ****************** END MOUSE EVENTS ***************** //

		default:
			break;
		}
	}
	return mbIsRunning;
}

SDL_Window* CoreSystem::Window::pGetWindow()
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
	return screenRect;
}
