#include "TextBox.h"

TextBox::TextBox(const Maths::IRect& rect)
	:
	rect(rect)
{
}

void TextBox::Draw(const std::string& text, const GraphicsEngine::Color& textColor, const GraphicsEngine::Color& boxColor, const GraphicsEngine::Color& backgroundColor)
{
	GraphicsEngine::Graphics::GetInstance().DrawFilledRect(rect, backgroundColor, 9);
	GraphicsEngine::Graphics::GetInstance().DrawRect(rect, boxColor, 9);
	GraphicsEngine::Font::GetInstance("ttf/arcardeClassic.ttf", 16).DrawText(rect.GetVec2DFrom(), text.c_str(), textColor);
}
