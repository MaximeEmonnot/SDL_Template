#include "Game.h"
#include <random>

Game::Game():
	mpWnd(CoreSystem::Window::GetInstance()),
	mpGfx(GraphicsEngine::Graphics::GetInstance())
{
	scenes.emplace_back(std::make_shared<TitleScene>("Images/background.png"));
	scenes.emplace_back(std::make_shared<ExplorationScene>("Images/background.png"));
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
	int output = -1;
	(*currentScene)->Update(output);

	switch (output) {
	case 0:
		mpWnd->ExitGame();
		break;
	case 1:
		currentScene++;
		break;
	case 2:
		break;
	default:
		break;
	}
}

void Game::RenderFrame()
{
	(*currentScene)->Draw();
}
