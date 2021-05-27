#pragma once
#include <SDL_pixels.h>
#define BLACK GraphicsEngine::Color(0, 0, 0, 255)
#define RED  GraphicsEngine::Color(255, 0, 0, 255)
#define GREEN  GraphicsEngine::Color(0, 255, 0, 255)
#define BLUE  GraphicsEngine::Color(0, 0, 255, 255)
#define MAGENTA GraphicsEngine::Color(255, 0, 255, 255)
#define CYAN GraphicsEngine::Color(0, 255, 255, 255)
#define YELLOW GraphicsEngine::Color(255, 255, 0, 255)
#define WHITE GraphicsEngine::Color(255, 255, 255, 255)
#define GRAY GraphicsEngine::Color(128, 128, 128, 255)
#define LIGHTGRAY GraphicsEngine::Color(185, 185, 185, 255)

namespace GraphicsEngine {
	class Color {
	public:
		Color()
			:
			c({ 0, 0, 0, 0 })
		{}
		Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
			:
			c({ red, green, blue, alpha })
		{}

		inline Color BlendColor(const Color& rhs, float percentage) {
			c.r = Uint8((rhs.c.r - c.r) * percentage + c.r);
			c.g = Uint8((rhs.c.g - c.g) * percentage + c.g);
			c.b = Uint8((rhs.c.b - c.b) * percentage + c.b);
			c.a = Uint8((rhs.c.a - c.a) * percentage + c.a);
			return *this;
		}

	public:
		SDL_Color c;
	};
}