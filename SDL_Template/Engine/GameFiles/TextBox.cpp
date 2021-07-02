#include "TextBox.h"

TextBox::TextBox(const std::string& text, const Maths::IRect& rect)
	:
	pGfx(GraphicsEngine::Graphics::GetInstance()),
	rect(rect),
	text(text)
{
}

void TextBox::Draw(GraphicsEngine::Font& font)
{
	font.DrawText(rect.GetCenterOfRect(), text.c_str(), BLUE);
	pGfx->DrawRect(rect, RED);
}
