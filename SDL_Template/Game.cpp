#include "Game.h"
#include <iostream>

Game::Game(Window& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	kirby("json\\kirby.json", gfx),
	destKirby({0, 0, 64, 64})
{
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
	float dt = t.DeltaTime();
	kirby.Update(dt);

	destKirby.x = wnd.mouse.GetMousePosX(), destKirby.y = wnd.mouse.GetMousePosY();

	auto e = wnd.mouse.Read();
	switch (e) {
	case Mouse::EventType::LPress:
		kirby.LastAnimation();
		break;
	case Mouse::EventType::RPress:
		kirby.NextAnimation();
		break;
	case Mouse::EventType::WheelUp:
		destKirby.w--, destKirby.h--;
		break;
	case Mouse::EventType::WheelDown:
		destKirby.w++, destKirby.h++;
		break;
	default:
		break;
	}
}

void Game::RenderFrame()
{
	kirby.Draw(destKirby, gfx);
}
