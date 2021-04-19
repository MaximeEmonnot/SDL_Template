#include "Game.h"

Game::Game(Window& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	color({255, 255, 255, 255})
{
}

Game::~Game()
{
}

void Game::Go()
{
	gfx.BeginRender();
	CalculateFrame();
	RenderFrame();
	gfx.EndRender();
}

void Game::CalculateFrame()
{
	Mouse::Type type = wnd.mouse.Read();
	switch (type) {
	case Mouse::Type::LPress:
		color = { 0, 0, 0, 0 };
		break;
	case Mouse::Type::LRelease:
		color = { 0, 0, 255, 255 };
		break;
	case Mouse::Type::RPress:
		color = { 0, 255, 0, 255 };
		break;
	case Mouse::Type::RRelease:
		color = { 255, 0, 0, 255 };
		break;
	case Mouse::Type::WheelUp:
		color = { 255, 255, 0, 255 };
		break;
	case Mouse::Type::WheelDown:
		color = { 255, 0, 255, 255 };
		break;
	case Mouse::Type::Move:
		color = { 0, 255, 255, 255 };
		break;
	case Mouse::Type::None:
		color = { 255, 255, 255, 255 };
		break;
	default:
		break;
	}
}

void Game::RenderFrame()
{
	gfx.RenderDrawRectTest({wnd.mouse.GetMousePosX(), wnd.mouse.GetMousePosY(), 50, 50},color);
}
