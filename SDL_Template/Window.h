#pragma once
#include "Timer.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Graphics.h"
#include "SoundSystem.h"
#include "SDLException.h"

class Window {
private:
	friend Graphics::Graphics(Window&);
private:
	class Exception : public SDLException {
	public:
		Exception(const std::string& file, unsigned int line, const std::string& note) noexcept
			:
			SDLException(file, line, note)
		{}
		inline std::string GetType() const noexcept override {
			return "SDL Window Exception caught";
		}
	};
public:
	Window();
	~Window();

	bool		EventListener() noexcept;

public:
	Timer       t;
	Mouse		mouse;
	Keyboard	kbd;
	SoundSystem sSystem;

private:
	SDL_Window* window;
	SDL_Event	event;
	bool        running;
};