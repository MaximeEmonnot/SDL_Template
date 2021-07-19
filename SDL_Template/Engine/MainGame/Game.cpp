#include "Game.h"
#include <random>

Game::Game():
	mpWnd(CoreSystem::Window::GetInstance()),
	mpGfx(GraphicsEngine::Graphics::GetInstance())
{
	scenes.emplace_back(std::make_shared<TitleScene>("Images/background.png"));
	scenes.emplace_back(std::make_shared<ChoosingScene>("Images/background.png"));
	scenes.emplace_back(std::make_shared<ExplorationScene>("Images/background.png"));
	scenes.emplace_back(std::make_shared<FightingScene>("Images/background.png"));
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

	if ((*currentScene)->ChangeScene()) currentScene += std::ptrdiff_t((int)(*currentScene)->GetNextScene() - (int)(*currentScene)->GetCurrentScene());
}

void Game::RenderFrame()
{
	(*currentScene)->Draw();
}
