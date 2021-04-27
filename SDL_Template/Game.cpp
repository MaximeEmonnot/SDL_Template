#include "Game.h"
#include <random>

Game::Game(Window& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	kbdWidth(wnd.kbd.GetKeyboardWidth()),
	kbdHeight(wnd.kbd.GetKeyboardHeight()),
	squarePlayer({ 50, kbdHeight - 50, 10, 25 }),
	squareEnemy({ kbdWidth, 15, 20, 20 })
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

void Game::ComputeCorsairColors()
{
	if (lost){
		for (float i = 0.0f; i < M_PI; i += 0.1f) {
			wnd.kbd.SetKeyboardColor(SDL_Color({ Uint8(255 * sin(i)), 0, 0, 255 }));
			if (!lost) break;
		}
	}
	else {
		for (auto& rect : wnd.kbd.GetKeyboardRect()) {
			if (SDL_HasIntersection(&rect, &squarePlayer)) {
				wnd.kbd.FadeKeyColorTo(rect, BLUE);
			}
			else if (SDL_HasIntersection(&rect, &squareEnemy)) {
				wnd.kbd.FadeKeyColorTo(rect, RED);
			}
			else {
				wnd.kbd.FadeKeyColorTo(rect, BLACK);
			}
		}
	}
}

void Game::UpdateFrame()
{
	auto e = wnd.kbd.ReadKey();
	if (!lost) {
		if (squarePlayer.y < kbdHeight - 50) {
			impulsionForce -= 0.1f;
			squarePlayer.y -= impulsionForce;
		}
		else {
			impulsionForce = 3.0f;
			if (e.GetCode() == SDL_SCANCODE_SPACE) {
				squarePlayer.y -= impulsionForce;
			}
		}

		squareEnemy.x -= 5;
		if (squareEnemy.x < 0) {
			std::mt19937 rng(std::random_device{}());
			std::uniform_int_distribution<int> posYDist(50, kbdHeight - 10);
			std::uniform_int_distribution<int> spawnRate(0, 10);
			if (spawnRate(rng) == 1) {
				squareEnemy.x = kbdWidth;
				squareEnemy.y = posYDist(rng);
			}
		}
		else {
			if (SDL_HasIntersection(&squarePlayer, &squareEnemy)) {
				lost = true;
				squareEnemy.x = kbdWidth;
				squarePlayer.y = kbdHeight - 50;
			}
		}
	}
	else {
		if (wnd.kbd.KeyIsPressed(SDL_SCANCODE_RETURN)) {
			lost = false;
		}
	}
}

void Game::RenderFrame()
{
}
