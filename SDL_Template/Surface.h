#pragma once
#include <SDL.h>
#include <SDL_image.h>
class Surface
{
public:
	Surface();
	Surface(const char* path, SDL_Renderer* renderer);
	Surface(SDL_Surface* surf, SDL_Renderer* renderer, int width, int height);
	Surface(const Surface&);
	~Surface();

	SDL_Texture* GetTexture() const;
	int GetWidth() const;
	int GetHeight() const;

private:
	SDL_Texture* tex;
	int width = 0;
	int height = 0;
};

