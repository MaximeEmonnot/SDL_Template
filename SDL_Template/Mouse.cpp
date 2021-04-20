#include "Mouse.h"

Mouse::EventType Mouse::Read()
{
	if (buffer.size() > 0u) {
		auto e = buffer.front();
		buffer.pop();
		return e;
	}
	else {
		return EventType::None;
	}
}

int Mouse::GetMousePosX() const
{
	return x;
}

int Mouse::GetMousePosY() const
{
	return y;
}

IVec2D Mouse::GetMousePos() const
{
	return IVec2D(x, y);
}

bool Mouse::LeftIsPressed() const
{
	return leftIsPressed;
}

bool Mouse::RightIsPressed() const
{
	return rightIsPressed;
}

void Mouse::OnMouseMove(int newx, int newy)
{
	x = newx, y = newy;
	buffer.push(EventType::Move);
	TrimBuffer();
}

void Mouse::OnLeftPressed()
{
	leftIsPressed = true;
	buffer.push(EventType::LPress);
	TrimBuffer();
}

void Mouse::OnLeftReleased()
{
	leftIsPressed = false;
	buffer.push(EventType::LRelease);
	TrimBuffer();
}

void Mouse::OnRightPressed()
{
	rightIsPressed = true;
	buffer.push(EventType::RPress);
	TrimBuffer();
}

void Mouse::OnRightReleased()
{
	rightIsPressed = false;
	buffer.push(EventType::RRelease);
	TrimBuffer();
}

void Mouse::OnWheelUp()
{
	buffer.push(EventType::WheelUp);
	TrimBuffer();
}

void Mouse::OnWheelDown()
{
	buffer.push(EventType::WheelDown);
	TrimBuffer();
}

void Mouse::TrimBuffer()
{
	if (buffer.size() > sizeBuffer) {
		buffer.pop();
	}
}
