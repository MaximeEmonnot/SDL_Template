#pragma once
#include "Font.h"
#include "Graphics.h"

class TextBox {
public:
	TextBox(const Maths::IRect& rect);

	void Draw(const std::string& text, const GraphicsEngine::Color& textColor, const GraphicsEngine::Color& boxColor, const GraphicsEngine::Color& backgroundColor);

private:
	std::shared_ptr<GraphicsEngine::Font> pFont;
	std::shared_ptr<GraphicsEngine::Graphics> pGfx;
	Maths::IRect rect;
};
