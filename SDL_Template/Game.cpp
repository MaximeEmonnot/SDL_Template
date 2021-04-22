#include "Game.h"

Game::Game(Window& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
}

Game::~Game()
{}

void Game::Go()
{
	gfx.BeginRender();
	UpdateFrame();
	RenderFrame();
	gfx.EndRender();
}

void Game::UpdateFrame()
{
}

void Game::RenderFrame()
{
}
