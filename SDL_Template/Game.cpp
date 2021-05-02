#include "Game.h"
#include <random>

Game::Game(Window& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	kbdWidth(wnd.kbd.GetKeyboardWidth()),
	kbdHeight(wnd.kbd.GetKeyboardHeight()),
	squarePlayer({ 50, kbdHeight - 50, 10, 25 }),
	squareEnemy({ kbdWidth, 15, 20, 20 })
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
