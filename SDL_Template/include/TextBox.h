#pragma once
#include "Font.h"
#include "Graphics.h"

class TextBox {
public:
	TextBox(GraphicsEngine::Font& font, const Maths::IRect& rect);

	void Draw(const std::string& text, const GraphicsEngine::Color& textColor, const GraphicsEngine::Color& boxColor);

private:
	GraphicsEngine::Font font;
	std::shared_ptr<GraphicsEngine::Graphics> pGfx;
	Maths::IRect rect;
};
