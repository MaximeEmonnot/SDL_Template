#include "Game.h"

Game::Game(Window& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
}

Game::~Game()
{
}

void Game::Go()
{
	gfx.BeginRender();
	CalculateFrame();
	RenderFrame();
	gfx.EndRender();
}

void Game::CalculateFrame()
{
}

void Game::RenderFrame()
{
	gfx.RenderColorTest(0, 255, 0, 255);
	gfx.RenderDrawRectTest({ 150, 150, 56, 56 }, {255, 0, 0, 255});
}
