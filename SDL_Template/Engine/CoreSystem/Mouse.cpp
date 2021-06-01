#include "Mouse.h"

CoreSystem::Mouse::~Mouse()
{
	Flush();
}

CoreSystem::Mouse::EventType CoreSystem::Mouse::Read()
{
	if (mBuffer.size() > 0u) {
		auto e = mBuffer.front();
		mBuffer.pop();
		return e;
	}
	else {
		return EventType::None;
	}
}

int CoreSystem::Mouse::GetMousePosX() const
{
	return mX;
}

int CoreSystem::Mouse::GetMousePosY() const
{
	return mY;
}

Maths::IVec2D CoreSystem::Mouse::GetMousePos() const
{
	return Maths::IVec2D(mX, mY);
}

bool CoreSystem::Mouse::LeftIsPressed() const
{
	return mbLeftIsPressed;
}

bool CoreSystem::Mouse::RightIsPressed() const
{
	return mbRightIsPressed;
}

void CoreSystem::Mouse::OnMouseMove(int newx, int newy)
{
	mX = newx, mY = newy;
	mBuffer.push(EventType::Move);
	TrimBuffer();
}

void CoreSystem::Mouse::OnLeftPressed()
{
	mbLeftIsPressed = true;
	mBuffer.push(EventType::LPress);
	TrimBuffer();
}

void CoreSystem::Mouse::OnLeftReleased()
{
	mbLeftIsPressed = false;
	mBuffer.push(EventType::LRelease);
	TrimBuffer();
}

void CoreSystem::Mouse::OnRightPressed()
{
	mbRightIsPressed = true;
	mBuffer.push(EventType::RPress);
	TrimBuffer();
}

void CoreSystem::Mouse::OnRightReleased()
{
	mbRightIsPressed = false;
	mBuffer.push(EventType::RRelease);
	TrimBuffer();
}

void CoreSystem::Mouse::OnWheelUp()
{
	mBuffer.push(EventType::WheelUp);
	TrimBuffer();
}

void CoreSystem::Mouse::OnWheelDown()
{
	mBuffer.push(EventType::WheelDown);
	TrimBuffer();
}

void CoreSystem::Mouse::TrimBuffer()
{
	if (mBuffer.size() > mSizeBuffer) {
		mBuffer.pop();
	}
}

void CoreSystem::Mouse::Flush()
{
	mBuffer = std::queue<Mouse::EventType>();
}
