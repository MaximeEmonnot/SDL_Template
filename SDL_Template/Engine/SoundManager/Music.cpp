#include "Music.h"

SoundEngine::Music::Music(const std::string& path)
{
	this->path = path;
	pMusic.reset(Mix_LoadMUS(path.c_str()), Mix_FreeMusic);
	if (pMusic == NULL)
		throw EngineException("SDL Sound System Exception caught", __FILE__, "An error has been caught during Music Initialisation.\nPlease check file path.", __LINE__);
}

void SoundEngine::Music::Play(int loops)
{
	Mix_FadeInMusic(pMusic.get(), loops, 50);
	Mix_PlayMusic(pMusic.get(), loops);
}

void SoundEngine::Music::Stop()
{
	Mix_FadeOutMusic(50);
}
