#include "Game.h"
#include <random>

Game::Game()
	:
	pPlayer(std::make_shared<Player>(Maths::IRect(384, 267, 32, 44), "json/player.json"))
{
	scenes.emplace_back(std::make_shared<TitleScene>(pPlayer));
	scenes.emplace_back(std::make_shared<ChoosingScene>(pPlayer));
	scenes.emplace_back(std::make_shared<ExplorationScene>(pPlayer));
	scenes.emplace_back(std::make_shared<FightingScene>(pPlayer));
	currentScene = scenes.begin();
}

void Game::Go()
{
	GraphicsEngine::Graphics::GetInstance().BeginRender();
	UpdateFrame();
	RenderFrame();
	GraphicsEngine::Graphics::GetInstance().EndRender();
}

void Game::ComputeCorsairColors()
{	
}

void Game::UpdateFrame()
{
	//ONLY CALLED ONCE
	CoreSystem::Timer::GetInstance().Update();

	(*currentScene)->Update();

	if ((*currentScene)->ChangeScene()) currentScene += std::ptrdiff_t(static_cast<long long>((*currentScene)->GetNextScene()) - static_cast<long long>((*currentScene)->GetCurrentScene()));
}

void Game::RenderFrame()
{
	(*currentScene)->Draw();
}
