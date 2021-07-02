#include "Game.h"
#include <random>

Game::Game():
	mpWnd(CoreSystem::Window::GetInstance()),
	mpGfx(GraphicsEngine::Graphics::GetInstance()),
	box("Test bro", Maths::IRect(Maths::IVec2D(400, 400), 100, 50)),
	player(Maths::IRect(Maths::IVec2D(75, 75), 64, 64), "json/kirby.json")
{
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
	player.Update(mpWnd->pTimer->DeltaTime());

	Maths::IVec2D dir;
	if (mpWnd->pKbd->KeyIsPressed(SDL_SCANCODE_LEFT)) {
		dir.x -= 1;
	}
	if (mpWnd->pKbd->KeyIsPressed(SDL_SCANCODE_UP)) {
		dir.y -= 1;
	}
	if (mpWnd->pKbd->KeyIsPressed(SDL_SCANCODE_RIGHT)) {
		dir.x += 1;
	}
	if (mpWnd->pKbd->KeyIsPressed(SDL_SCANCODE_DOWN)) {
		dir.y += 1;
	}
	player.Move(dir);
}

void Game::RenderFrame()
{
	player.Draw();
	box.Draw(font);
}
