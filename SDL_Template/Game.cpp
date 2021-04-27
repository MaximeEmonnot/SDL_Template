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

void Game::ComputeCorsairColors()
{
	for (auto rect : wnd.kbd.GetKeyboardRect()) {
		if (SDL_HasIntersection(, rect)) {
			wnd.kbd.FadeKeyColorTo(rect, RED, 0.25f);
		}
	}
}

void Game::UpdateFrame()
{

}

void Game::RenderFrame()
{

}
