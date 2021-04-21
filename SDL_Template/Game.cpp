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

	char c = wnd.kbd.ReadChar();

	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= SDLK_0 && c <= SDLK_9) || c == SDLK_SPACE) {
		text += c;
	}
	else if (c == SDLK_BACKSPACE && text.size() > 0) {
		text.pop_back();
	}
}

void Game::RenderFrame()
{
	gfx.SetBackgroundColor({ 255, 255, 255, 255 });
	kirby.Draw(destKirby, gfx);

	if (text.size() > 0) {
		font.DrawLoadedText(155, 155, text.c_str(), { 0, 0, 0, 255 });
	}
}
