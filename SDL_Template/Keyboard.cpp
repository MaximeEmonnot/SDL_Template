#include "Keyboard.h"
#include "WindowManager.h"

Keyboard::Keyboard(Window& wnd)
	:
	event(wnd.GetEvent())
{
}

bool Keyboard::KeyIsPressed(SDL_Keycode kCode) const
{
	return keystates[kCode];
}

Keyboard::Event Keyboard::ReadKey()
{
	if (bufferEvents.size() > 0u) {
		Keyboard::Event e = bufferEvents.front();
		bufferEvents.pop();
		return e;
	}
	return Keyboard::Event();
}

char Keyboard::ReadChar()
{
	if (bufferChar.size() > 0u) {
		char c = bufferChar.front();
		bufferChar.pop();
		return c;
	}
	return 0;
}

bool Keyboard::KeyIsEmpty() const
{
	return bufferEvents.empty();
}

void Keyboard::OnKeyPressed(SDL_Keycode code)
{
	keystates[code] = true;
	bufferEvents.push(Event(code, Event::Type::Pressed));
	TrimBuffer(bufferEvents);
}

void Keyboard::OnKeyReleased(SDL_Keycode code)
{
	keystates[code] = false;
	bufferEvents.push(Event(code, Event::Type::Released));
	TrimBuffer(bufferEvents);
}

void Keyboard::OnChar(SDL_Keycode code)
{
	bufferChar.push(code);
	TrimBuffer(bufferChar);
}

void Keyboard::Flush()
{
	keystates.reset();
	bufferEvents = std::queue<Event>();
	bufferChar = std::queue<char>();
}

template <typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer)
{
	if (buffer.size() > bufferSize) {
		buffer.pop();
	}
}
