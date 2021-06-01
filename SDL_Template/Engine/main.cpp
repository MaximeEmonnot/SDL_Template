#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <thread>
#include "Game.h"

int main(int argc, char* argv[])
{
	try {
		std::shared_ptr<CoreSystem::Window> wnd = CoreSystem::Window::GetInstance();
		try {
			Game theGame;
			std::thread t;
			if (wnd->pKbd->IsCorsairKeyboard()) {
				auto gameCCC = [&]() { while (wnd->ListensToEvents()) { theGame.ComputeCorsairColors(); } };
				t = std::thread(gameCCC);
			}
			while (wnd->ListensToEvents()) {
				theGame.Go();
			}
			if (t.joinable()) t.join();
		}
		catch (const SDLException& e) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, e.GetType().c_str(), e.GetMessage().c_str(), NULL);
		}
		catch (const std::exception& e) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "STL Exception caught", (std::string("A STL Exception has been caught during Game Routine.\nDetails:\n") + std::string(e.what())).c_str(), NULL);
		}
		catch (...) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Unknown error caught", "An unknown error has been caught during Game Routine.", NULL);
		}
		wnd->Kill();
	}
	catch (const SDLException& e) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, e.GetType().c_str(), e.GetMessage().c_str(), NULL);
	}
	catch (const std::exception& e) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "STL Exception caught", (std::string("A STL Exception has been caught during Window Creation.\nDetails:\n") + std::string(e.what())).c_str(), NULL);
	}
	catch (...) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Unknown error caught", "An unknown error has been caught during Window Creation.", NULL);
	}

#if _DEBUG
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}