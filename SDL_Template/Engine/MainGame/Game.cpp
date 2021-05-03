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
	for (auto& rect : wnd.kbd.GetKeyboardRect()) {
		wnd.kbd.SetKeyColorByRect(rect, RED);
	}
}

void Game::UpdateFrame()
{
}

void Game::RenderFrame()
{
}
