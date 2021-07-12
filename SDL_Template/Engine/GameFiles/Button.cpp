#include "Button.h"
#include <iostream>

Button::Button(Maths::IVec2D pos, std::string text, std::function<void(int&)> func)
	:
	rect(pos, (int)text.size() * 8, 16),
	text(text),
	function(func),
	pGfx(GraphicsEngine::Graphics::GetInstance())
{
}

Button::~Button()
{
	pGfx = nullptr;
}

bool Button::OnClick(Maths::IVec2D mousePos)
{
	if (rect.ContainsVec2D(mousePos)) {
		bIsClicked = true;
		return true;
	}
	return false;
}

bool Button::OnHover(Maths::IVec2D mousePos) const
{
	return rect.ContainsVec2D(mousePos);
}

void Button::ComputeFunction(int& output) const
{
	function(output);
}

void Button::Draw(GraphicsEngine::Color buttonColor, GraphicsEngine::Font& font)
{
	pGfx->DrawFilledRect(rect, buttonColor);
	font.DrawText(rect.GetVec2DFrom(), text.c_str(), BLACK);
}

bool Button::GetState()
{
	bool state = bIsClicked;
	bIsClicked = false;
	return state;
}
