#pragma once
#include <SDL.h>
#include "SingletonMaker.h"
#include "Timer.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "ThreadPool.h"
#include "SoundSystem.h"
#include "EngineException.h"

namespace CoreSystem {
	class Window : public SingletonMaker<Window> {
	public:
		Window();
		~Window();

		bool							ListensToEvents() noexcept;

		void							ExitGame();

		std::shared_ptr<SDL_Window>		pGetWindow();
		const int						GetWidth() const;
		const int						GetHeight() const;
		const Maths::IRect				GetScreenRect() const;

		bool							IsMinimized() const;

	private:
		std::shared_ptr<SDL_Window>					mpWindow;
		SDL_Event									mEvent;
		bool										mbIsRunning;
		bool										mbIsMinimized;
		const int									mWidth = 800;
		const int									mHeight = 600;
		const Maths::IRect							mScreenRect = { 0, 0, mWidth, mHeight };
	};
}