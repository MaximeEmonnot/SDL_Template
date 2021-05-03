#include "Effect.h"

SoundEngine::Effect::Effect(const char* path, int index)
{
	this->index = index;
	effect = Mix_LoadWAV(path);
	if (effect == NULL)
		throw SDLException("SDL Sound System Exception caught", __FILE__, __LINE__, "An error has been caught during SoundEffect Initialisation.\nPlease check file path.");
}

SoundEngine::Effect::~Effect()
{
	Mix_FreeChunk(effect);
}

void SoundEngine::Effect::Play(int loops)
{
	Mix_PlayChannel(-1, effect, loops);
}

void SoundEngine::Effect::Stop()
{
	Mix_Pause(-1);
}

int SoundEngine::Effect::GetIndex() const
{
	return index;
}

