#pragma once
#include <SDL_mixer.h>
#include "SDLException.h"
class Music {
public:
	Music(const char* path, int index);
	~Music();

	void Play(int loops);
	void Stop();
	int GetIndex() const;
private:
	int index = 0;
	Mix_Music* music;
};