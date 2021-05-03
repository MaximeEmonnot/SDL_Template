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