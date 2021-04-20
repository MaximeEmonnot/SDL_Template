#include "Game.h"
#include <iostream>

Game::Game(Window& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	color({255, 255, 255, 255})
{
}

Game::~Game()
{}

void Game::Go()
{
	gfx.BeginRender();
	CalculateFrame();
	RenderFrame();
	gfx.EndRender();
}

void Game::CalculateFrame()
{
	color = { 0, 0 ,0 ,255 };
	if (wnd.kbd.KeyIsPressed(SDLK_a))
		color.r = 255;
	if (wnd.kbd.KeyIsPressed(SDLK_z))
		color.g = 255;
	if (wnd.kbd.KeyIsPressed(SDLK_e))
		color.b = 255;

	std::cout << "(" << wnd.mouse.GetMousePosX() << "," << wnd.mouse.GetMousePosY() << ")\n";
}

void Game::RenderFrame()
{
	gfx.RenderColorTest(color);
}
