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

		bool		EventListener() noexcept;

		SDL_Window* GetWindow();
		const int	GetWidth() const;
		const int   GetHeight() const;
		const Maths::IRect GetScreenRect() const;

	public:
		std::shared_ptr<Timer>       t;
		std::shared_ptr<Mouse>		mouse;
		std::shared_ptr<Keyboard>	kbd;
		std::shared_ptr<SoundEngine::SoundSystem> sSystem;

	private:
		SDL_Window* window;
		SDL_Event	event;
		bool        running;
		const int	width = 800;
		const int   height = 600;
		const Maths::IRect screenRect = { 0, 0, width, height };
	};
}