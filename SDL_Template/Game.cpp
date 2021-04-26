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
			currentIndex = 8;
		else {
			currentIndex--;
		}
	}
		break;
	case Mouse::EventType::WheelDown:
	{
		if (currentIndex == 8)
			currentIndex = 0;
		else {
			currentIndex++;
		}
	}
		break;
	default:
		break;
	}

	wnd.kbd.SetKeyColorByPosition(wnd.mouse.GetMousePos(), colors[currentIndex]);

	auto e = wnd.kbd.ReadKey();
	if (e.GetCode() != SDL_SCANCODE_UNKNOWN)
		circles.push_back(FCircle(FVec2D(wnd.kbd.GetKeyRect(e.GetCode()).x, wnd.kbd.GetKeyRect(e.GetCode()).y), 1));
	if (circles.size() != 0) {
		std::vector<FCircle>::iterator toDelete = circles.end();
		for (int i = 0; i < circles.size(); i++) {
			for (auto rect : wnd.kbd.GetKeyboardRect()) {
				if (RectIsOverlappedByCircle(rect, circles[i])) wnd.kbd.FadeKeyColorTo(rect, BLACK);
			}
			circles[i].radius += 1;
			if (circles[i].radius > 50)
				toDelete = circles.begin() + i;
		}
		if (toDelete != circles.end())
			circles.erase(toDelete);
	}
}

void Game::RenderFrame()
{
}
