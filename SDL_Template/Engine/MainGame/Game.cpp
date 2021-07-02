#include "Game.h"
#include <random>

Game::Game():
	mpWnd(CoreSystem::Window::GetInstance()),
	mpGfx(GraphicsEngine::Graphics::GetInstance()),
	menu(new MiddleMenu(new TopMenu(new LeftMenu(new BottomMenu(new RightMenu(new BasicMenu()))))))
{
}

Game::~Game()
{
	menu->Destroy();
	delete menu;
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
	menu->Update(mpWnd->pMouse);
}

void Game::RenderFrame()
{
	menu->Draw(font);
}
