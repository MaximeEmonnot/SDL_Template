#include "Mouse.h"

CoreSystem::Mouse::EventType CoreSystem::Mouse::Read()
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

int CoreSystem::Mouse::GetMousePosX() const
{
	return x;
}

int CoreSystem::Mouse::GetMousePosY() const
{
	return y;
}

Maths::IVec2D CoreSystem::Mouse::GetMousePos() const
{
	return Maths::IVec2D(x, y);
}

bool CoreSystem::Mouse::LeftIsPressed() const
{
	return leftIsPressed;
}

bool CoreSystem::Mouse::RightIsPressed() const
{
	return rightIsPressed;
}

void CoreSystem::Mouse::OnMouseMove(int newx, int newy)
{
	x = newx, y = newy;
	buffer.push(EventType::Move);
	TrimBuffer();
}

void CoreSystem::Mouse::OnLeftPressed()
{
	leftIsPressed = true;
	buffer.push(EventType::LPress);
	TrimBuffer();
}

void CoreSystem::Mouse::OnLeftReleased()
{
	leftIsPressed = false;
	buffer.push(EventType::LRelease);
	TrimBuffer();
}

void CoreSystem::Mouse::OnRightPressed()
{
	rightIsPressed = true;
	buffer.push(EventType::RPress);
	TrimBuffer();
}

void CoreSystem::Mouse::OnRightReleased()
{
	rightIsPressed = false;
	buffer.push(EventType::RRelease);
	TrimBuffer();
}

void CoreSystem::Mouse::OnWheelUp()
{
	buffer.push(EventType::WheelUp);
	TrimBuffer();
}

void CoreSystem::Mouse::OnWheelDown()
{
	buffer.push(EventType::WheelDown);
	TrimBuffer();
}

void CoreSystem::Mouse::TrimBuffer()
{
	if (buffer.size() > sizeBuffer) {
		buffer.pop();
	}
}

void CoreSystem::Mouse::Flush()
{
	buffer = std::queue<Mouse::EventType>();
}
