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
		Exception(const std::string& file, unsigned int line, const std::string& note)
			:
			SDLException(file, line, note)
		{}
		std::string GetType() const override {
			return "SDL Window Exception caught";
		}
	};
public:
	Window();
	~Window();

	void		EventListener();

	SDL_Window* GetWindow();
	bool		IsRunning() const;

public:
	Mouse		mouse;
	Keyboard	kbd;

private:
	SDL_Window* window;
	SDL_Event	event;
	bool		running;
};