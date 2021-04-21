#include "Game.h"
#include <iostream>

Game::Game(Window& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	kirby("json\\kirby.json", gfx),
	destKirby({0, 0, 64, 64}),
	font("ttf\\arcadeClassic.TTF", 25, gfx)
{
	sSystem.AddSound("music\\test_laser.wav", EFFECT);
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

	auto ek = wnd.kbd.ReadKey();
	if (ek.GetCode() == SDLK_SPACE) {
		sSystem.PlayAll(0);
	}
}

void Game::RenderFrame()
{
	gfx.SetBackgroundColor({ 255, 255, 255, 255 });
	kirby.Draw(destKirby, gfx);
}
