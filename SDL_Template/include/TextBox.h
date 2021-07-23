#pragma once
#include "Font.h"
#include "Graphics.h"

class TextBox {
public:
	TextBox(const std::string& text, const Maths::IRect& rect);

	void Draw(GraphicsEngine::Font& font, const GraphicsEngine::Color& textColor, const GraphicsEngine::Color& boxColor);

private:
	std::shared_ptr<GraphicsEngine::Graphics> pGfx;
	Maths::IRect rect;
	std::string text;
};
