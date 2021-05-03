#include "Effect.h"


Effect::Effect(const char* path, int index)
{
	this->index = index;
	effect = Mix_LoadWAV(path);
	if (effect == NULL)
		throw SDLException("SDL Sound System Exception caught", __FILE__, __LINE__, "An error has been caught during SoundEffect Initialisation.\nPlease check file path.");
}

Effect::~Effect()
{
	Mix_FreeChunk(effect);
}

void Effect::Play(int loops)
{
	Mix_PlayChannel(-1, effect, loops);
}

void Effect::Stop()
{
	Mix_Pause(-1);
}

int Effect::GetIndex() const
{
	return index;
}

