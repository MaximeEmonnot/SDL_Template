#include "Game.h"
#include <iostream>

Game::Game(Window& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	color({255, 255, 255, 255})
{
}

Game::~Game()
{}

void Game::Go()
{
	gfx.BeginRender();
	CalculateFrame();
	RenderFrame();
	gfx.EndRender();
}

void Game::CalculateFrame()
{
	if (wnd.kbd.KeyIsPressed(SDLK_a)) {
		color = { 255, 0, 0, 255 };
	}

	if (wnd.kbd.KeyIsEmpty()) {
		color = { 255, 255, 255, 255 };
	}

}

void Game::RenderFrame()
{
	gfx.RenderColorTest(color);
}
