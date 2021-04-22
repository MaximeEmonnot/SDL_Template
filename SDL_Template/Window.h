#pragma once
#include "Graphics.h"
#include "Mouse.h"
#include "Keyboard.h"
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
	Mouse		mouse;
	Keyboard	kbd;

private:
	SDL_Window* window;
	SDL_Event	event;
};