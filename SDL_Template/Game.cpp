#include "Game.h"
#include <iostream>

Game::Game(Window& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	color({255, 255, 255, 255}),
	test({100, 100, 50, 50})
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
	color = { 50, 50 ,50 ,255 };
	if (wnd.kbd.KeyIsPressed(SDLK_a))
		color.r = 255;
	if (wnd.kbd.KeyIsPressed(SDLK_z))
		color.g = 255;
	if (wnd.kbd.KeyIsPressed(SDLK_e))
		color.b = 255;

	if (wnd.kbd.KeyIsPressed(SDLK_UP))
		test.y--;
	if (wnd.kbd.KeyIsPressed(SDLK_DOWN))
		test.y++;
	if (wnd.kbd.KeyIsPressed(SDLK_RIGHT))
		test.x++;
	if (wnd.kbd.KeyIsPressed(SDLK_LEFT))
		test.x--;

	std::cout << "(" << wnd.mouse.GetMousePosX() << "," << wnd.mouse.GetMousePosY() << ")\n";
}

void Game::RenderFrame()
{
	gfx.DrawFilledRect(test, color);
}
