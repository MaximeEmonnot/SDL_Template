#include "Game.h"
#include <random>

Game::Game():
	mpWnd(CoreSystem::Window::GetInstance()),
	mpGfx(GraphicsEngine::Graphics::GetInstance())
{
	mpWnd->pSoundSystem->PlaySound(mpWnd->pSoundSystem->ConstructNewSong("music/gigachad.wav", MUSIC), 1);
}

Game::~Game()
{
	mpWnd->Kill();
	mpGfx->Kill();
}

void Game::Go()
{
	mpGfx->BeginRender();
	UpdateFrame();
	RenderFrame();
	mpGfx->EndRender();
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
