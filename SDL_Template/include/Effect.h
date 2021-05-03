#pragma once
#include <SDL_mixer.h>
#include "SDLException.h"
class Effect {
public:
	Effect(const char* path, int index);
	~Effect();

	void		Play(int loops);
	void		Stop();
	int			GetIndex() const;
private:
	int			index = 0;
	Mix_Chunk*	effect;
};