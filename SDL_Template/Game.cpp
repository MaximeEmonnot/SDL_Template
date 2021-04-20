#include "Game.h"
#include <iostream>

Game::Game(Window& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	test("Images\\black_dot.png",gfx.GetRenderer()),
	testSurface({100, 100, test.GetWidth(), test.GetHeight()})
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
	if (wnd.kbd.KeyIsPressed(SDLK_z))
		testSurface.y--;
	if (wnd.kbd.KeyIsPressed(SDLK_q))
		testSurface.x--;
	if (wnd.kbd.KeyIsPressed(SDLK_s))
		testSurface.y++;
	if (wnd.kbd.KeyIsPressed(SDLK_d))
		testSurface.x++;
	if (wnd.kbd.KeyIsPressed(SDLK_a))
		testSurface.w--, testSurface.h--;
	if (wnd.kbd.KeyIsPressed(SDLK_e))
		testSurface.w++, testSurface.h++;


	std::cout << "(" << wnd.mouse.GetMousePosX() << "," << wnd.mouse.GetMousePosY() << ")\n";
}

void Game::RenderFrame()
{
	gfx.DrawFilledRect({ 250, 250, 50, 50 }, { 255, 0, 0, 255 });
	gfx.DrawSprite(testSurface, test);
}
