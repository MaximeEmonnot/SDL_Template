#include "Music.h"

SoundEngine::Music::Music(const char* path)
{
	this->path = path;
	music = Mix_LoadMUS(path);
	if (music == NULL)
		throw SDLException("SDL Sound System Exception caught", __FILE__, __LINE__, "An error has been caught during Music Initialisation.\nPlease check file path.");
}

SoundEngine::Music::~Music()
{
	Mix_FreeMusic(music);
}

void SoundEngine::Music::Play(int loops)
{
	Mix_PlayMusic(music, loops);
}

void SoundEngine::Music::Stop()
{
	Mix_PauseMusic();
}
