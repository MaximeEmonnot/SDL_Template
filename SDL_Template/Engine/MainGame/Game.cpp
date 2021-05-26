#include "Game.h"
#include <random>

Game::Game(CoreSystem::Window& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	wnd.sSystem.PlaySound(wnd.sSystem.ConstructNewSong("music\\gigachad.wav", MUSIC), 1);
	wnd.sSystem.PlaySound(wnd.sSystem.ConstructNewSong("music\\gigachad.wav", MUSIC), 1);
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
	int test = 4154561;
}

void Game::RenderFrame()
{
}
