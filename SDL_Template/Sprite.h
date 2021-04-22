#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "SDLException.h"
class Sprite
{
private:
	class Exception : public SDLException {
	public:
		Exception(const std::string& file, unsigned int line, const std::string& note) noexcept
			:
			SDLException(file, line, note)
		{}
		inline std::string GetType() const noexcept override {
			return "SDL Sprite Exception";
		}
	};
public:
	Sprite();
	Sprite(const char* path, SDL_Renderer* renderer);
	Sprite(SDL_Surface* surf, SDL_Renderer* renderer, int width, int height);
	Sprite(const Sprite&);

	Sprite& operator= (const Sprite& rhs);
	~Sprite();

	void			InitSurface(const char* path, SDL_Renderer* renderer);

	SDL_Texture*	GetTexture() const;
	int				GetWidth() const;
	int				GetHeight() const;

private:
	SDL_Texture*	tex;
	int				width = 0;
	int				height = 0;
};

