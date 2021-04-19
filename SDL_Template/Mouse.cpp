#include "Mouse.h"
#include "WindowManager.h"
#include "SDL.h"

Mouse::Mouse(Window& wnd)
	:
	event(wnd.GetEvent()),
	type(Type::None)
{
}

Mouse::Type Mouse::Read()
{
	return type;
}

Sint32 Mouse::GetMousePosX()
{
	return event.button.x;
}

Sint32 Mouse::GetMousePosY()
{
	return event.button.y;
}

Vec2D<Sint32> Mouse::GetMousePos()
{
	return Vec2D<Sint32>(event.button.x, event.button.y);
}

bool Mouse::LeftIsPressed()
{
	return type == Type::LPress;
}

bool Mouse::RightIsPressed()
{
	return type == Type::RPress;
}

void Mouse::SetType(Type newType)
{
	type = newType;
}
