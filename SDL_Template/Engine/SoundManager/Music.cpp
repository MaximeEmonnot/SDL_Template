#include "Music.h"

Music::Music(const char* path, int index)
{
	this->index = index;
	music = Mix_LoadMUS(path);
	if (music == NULL)
		throw SDLException("SDL Sound System Exception caught", __FILE__, __LINE__, "An error has been caught during Music Initialisation.\nPlease check file path.");
}

Music::~Music()
{
	Mix_FreeMusic(music);
}

void Music::Play(int loops)
{
	Mix_PlayMusic(music, loops);
}

void Music::Stop()
{
	Mix_PauseMusic();
}

int Music::GetIndex() const
{
	return index;
}
