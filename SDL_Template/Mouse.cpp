#include "Mouse.h"
#include "WindowManager.h"
#include "SDL.h"

Mouse::Mouse(Window& wnd)
	:
	event(wnd.GetEvent())
{
}

Mouse::Type Mouse::Read()
{
	if (buffer.size() > 0u) {
		auto e = buffer.front();
		buffer.pop();
		return e;
	}
	else {
		return Type::None;
	}
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
	return leftIsPressed;
}

bool Mouse::RightIsPressed()
{
	return rightIsPressed;
}

void Mouse::OnMouseMove()
{
	buffer.push(Type::Move);
	TrimBuffer();
}

void Mouse::OnLeftPressed()
{
	leftIsPressed = true;
	buffer.push(Type::LPress);
	TrimBuffer();
}

void Mouse::OnLeftReleased()
{
	leftIsPressed = false;
	buffer.push(Type::LRelease);
	TrimBuffer();
}

void Mouse::OnRightPressed()
{
	rightIsPressed = true;
	buffer.push(Type::RPress);
	TrimBuffer();
}

void Mouse::OnRightReleased()
{
	rightIsPressed = false;
	buffer.push(Type::RRelease);
	TrimBuffer();
}

void Mouse::OnWheelUp()
{
	buffer.push(Type::WheelUp);
	TrimBuffer();
}

void Mouse::OnWheelDown()
{
	buffer.push(Type::WheelDown);
	TrimBuffer();
}

void Mouse::TrimBuffer()
{
	if (buffer.size() > sizeBuffer) {
		buffer.pop();
	}
}
