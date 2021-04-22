#include "Game.h"
#include <iostream>

Game::Game(Window& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	kirby = std::make_unique<Character>("json\\kirby.json", gfx, SDL_Rect{ 0, 0, 64, 64 });
	wnd.sSystem.AddSound("music\\test_laser.wav", EFFECT);
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
	case Mouse::EventType::WheelUp:
		kirby->ZoomOut();
		break;
	case Mouse::EventType::WheelDown:
		kirby->ZoomIn();
		break;
	default:
		break;
	}

	auto ek = wnd.kbd.ReadKey();
	if (ek.GetCode() == SDLK_SPACE) {
		wnd.sSystem.PlayOneEffect(0, 0);
	}
}

void Game::RenderFrame()
{
	gfx.SetBackgroundColor({ 255, 255, 255, 255 });
	kirby->Draw(gfx);
}
