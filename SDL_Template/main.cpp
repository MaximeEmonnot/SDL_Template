#include "Game.h"

int main(int argc, char* argv[])
{
	try {
		Window wnd;
		try {
			Game theGame(wnd);
			while (wnd.EventListener()) {
				theGame.Go();
			}
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

	return 0;
}