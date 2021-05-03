#pragma once
#include <SDL_pixels.h>
#define BLACK SDL_Color({0, 0, 0, 255})
#define RED  SDL_Color({255, 0, 0, 255})
#define GREEN  SDL_Color({0, 255, 0, 255})
#define BLUE  SDL_Color({0, 0, 255, 255})
#define MAGENTA SDL_Color({255, 0, 255, 255})
#define CYAN SDL_Color({0, 255, 255, 255})
#define YELLOW SDL_Color({255, 255, 0, 255})
#define WHITE SDL_Color({255, 255, 255, 255})
#define GRAY SDL_Color({128, 128, 128, 255})
#define LIGHTGRAY SDL_Color({185, 185, 185, 255})

template <typename T>
SDL_Color BlendColor(const SDL_Color& c0, const SDL_Color& c1, T percentage) {
	SDL_Color output = { Uint8((c1.r - c0.r) * percentage + c0.r), Uint8((c1.g - c0.g) * percentage + c0.g), Uint8((c1.b - c0.b) * percentage + c0.b), Uint8((c1.a - c0.a) * percentage + c0.a) };
	return output;
}