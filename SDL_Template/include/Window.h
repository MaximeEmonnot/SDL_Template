#pragma once
#include <SDL.h>
#include "SingletonMaker.h"
#include "Timer.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "SoundSystem.h"
#include "SDLException.h"

namespace CoreSystem {
	class Window : public SingletonMaker<Window> {
	public:
		Window();
		~Window();

		bool		ListensToEvents() noexcept;

		SDL_Window* pGetWindow();
		const int	GetWidth() const;
		const int   GetHeight() const;
		const Maths::IRect GetScreenRect() const;

	public:
		std::shared_ptr<Timer>      pTimer;
		std::shared_ptr<Mouse>		pMouse;
		std::shared_ptr<Keyboard>	pKbd;
		std::shared_ptr<SoundEngine::SoundSystem> pSoundSystem;

	private:
		SDL_Window* mpWindow;
		SDL_Event	mEvent;
		bool        mbIsRunning;
		const int	mWidth = 800;
		const int   mHeight = 600;
		const Maths::IRect screenRect = { 0, 0, mWidth, mHeight };
	};
}