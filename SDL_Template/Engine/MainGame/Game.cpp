#include "Game.h"
#include <random>

Game::Game():
	wnd(CoreSystem::Window::GetInstance()),
	gfx(GraphicsEngine::Graphics::GetInstance()),
	sSystem(SoundEngine::SoundSystem::GetInstance())
{
	sSystem->PlaySound(sSystem->ConstructNewSong("music\\gigachad.wav", MUSIC), 1);
}

Game::~Game()
{
	wnd->Kill();
	gfx->Kill();
	sSystem->Kill();
}

void Game::Go()
{
	gfx->BeginRender();
	UpdateFrame();
	RenderFrame();
	gfx->EndRender();
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
