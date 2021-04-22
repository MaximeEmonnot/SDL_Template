#include "Game.h"

Game::Game(Window& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	kirby = std::make_unique<Character>("json\\kirby.json", gfx, SDL_Rect({ 0, 0, 64, 64 }));
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

void Game::UpdateFrame()
{
	float dt = wnd.t.DeltaTime();
	kirby->Update(dt);
	kirby->SetPos(wnd.mouse.GetMousePos());

	auto e = wnd.mouse.Read();
	switch (e) {
	case Mouse::EventType::LPress:
		kirby->LastAnimation();
		break;
	case Mouse::EventType::RPress:
		kirby->NextAnimation();
		break;
	case Mouse::EventType::WheelDown:
		kirby->ZoomIn();
		break;
	case Mouse::EventType::WheelUp:
		kirby->ZoomOut();
		break;
	default:
		break;
	}
}

void Game::RenderFrame()
{
	gfx.SetBackgroundColor(WHITE);
	kirby->Draw(gfx);
}
