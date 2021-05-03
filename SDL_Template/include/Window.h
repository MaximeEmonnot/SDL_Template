#pragma once
#include <SDL.h>
#include "Timer.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "SoundSystem.h"
#include "SDLException.h"

class Window {
public:
	Window();
	~Window();

	bool		EventListener() noexcept;

	SDL_Window* GetWindow();
	const int	GetWidth() const;
	const int   GetHeight() const;
	const SDL_Rect GetScreenRect() const;

public:
	Timer       t;
	Mouse		mouse;
	Keyboard	kbd;
	SoundEngine::SoundSystem sSystem;

private:
	SDL_Window* window;
	SDL_Event	event;
	bool        running;
	const int	width = 800;
	const int   height = 600;
	const SDL_Rect screenRect = { 0, 0, width, height };
};