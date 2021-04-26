#include "Game.h"

Game::Game(Window& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	wnd.kbd.SetKeyColor(SDL_SCANCODE_W, WHITE);
	wnd.kbd.SetKeyColor(SDL_SCANCODE_A, WHITE);
	wnd.kbd.SetKeyColor(SDL_SCANCODE_S, WHITE);
	wnd.kbd.SetKeyColor(SDL_SCANCODE_D, WHITE);
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
	auto mE = wnd.mouse.Read();
	switch (mE) {
	case Mouse::EventType::WheelUp:
	{
		if (currentIndex == 0)
			currentIndex = 9;
		else {
			currentIndex--;
		}
	}
		break;
	case Mouse::EventType::WheelDown:
	{
		if (currentIndex == 9)
			currentIndex = 0;
		else {
			currentIndex++;
		}
	}
		break;
	default:
		break;
	}

	auto e = wnd.kbd.ReadKey();
	wnd.kbd.SetKeyColor(e.GetCode(), BLACK);
	wnd.kbd.SetKeyColorByPosition(wnd.mouse.GetMousePos(), colors[currentIndex]);
}

void Game::RenderFrame()
{
}
