#include "Game.h"
#include <random>

Game::Game():
	mpWnd(CoreSystem::Window::GetInstance()),
	mpGfx(GraphicsEngine::Graphics::GetInstance())
{
	scenes.emplace_back(std::make_shared<TitleScene>());
	scenes.emplace_back(std::make_shared<ChoosingScene>());
	scenes.emplace_back(std::make_shared<ExplorationScene>());
	scenes.emplace_back(std::make_shared<FightingScene>());
	currentScene = scenes.begin();
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
	//ONLY CALLED ONCE
	mpWnd->pTimer->Update();

	(*currentScene)->Update();

	if ((*currentScene)->ChangeScene()) currentScene += std::ptrdiff_t(static_cast<long long>((*currentScene)->GetNextScene()) - static_cast<long long>((*currentScene)->GetCurrentScene()));
}

void Game::RenderFrame()
{
	(*currentScene)->Draw();
}
