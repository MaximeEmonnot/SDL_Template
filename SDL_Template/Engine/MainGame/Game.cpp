#include "Game.h"
#include <random>

Game::Game(CoreSystem::Window& wnd)
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

void Game::ComputeCorsairColors()
{
}

void Game::UpdateFrame()
{
}

void Game::RenderFrame()
{
}
