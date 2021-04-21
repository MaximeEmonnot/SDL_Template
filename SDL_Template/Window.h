#pragma once
#include "Graphics.h"
#include "Mouse.h"
#include "Keyboard.h"

class Window {
private:
	friend Graphics::Graphics(Window&);
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