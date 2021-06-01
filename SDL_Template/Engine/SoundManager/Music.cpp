#include "Music.h"

SoundEngine::Music::Music(const char* path)
	:
	pMusic(Mix_LoadMUS(path))
{
	this->path = path;
	if (pMusic == NULL)
		throw SDLException("SDL Sound System Exception caught", __FILE__, "An error has been caught during Music Initialisation.\nPlease check file path.", __LINE__);
}

SoundEngine::Music::~Music()
{
	Mix_FreeMusic(pMusic);
}

void SoundEngine::Music::Play(int loops)
{
	Mix_PlayMusic(pMusic, loops);
}

void SoundEngine::Music::Stop()
{
	Mix_PauseMusic();
}
