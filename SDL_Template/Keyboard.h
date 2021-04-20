#pragma once
#include <SDL.h>
#include <queue>
#include <bitset>

class Keyboard
{
private:
	friend class Window;
public:
	class Event {
	public:
		enum class Type {
			Pressed,
			Released,
			None
		};
	public:
		Event() 
			:
			type(Type::None)
		{
		}
		Event(SDL_Keycode kCode, Type type)
			:
			keycode(kCode),
			type(type) 
		{
		}
		bool IsPressed() const {
			return type == Type::Pressed;
		}
		bool IsReleased() const {
			return type == Type::Released;
		}
		SDL_Keycode GetCode() const {
			return keycode;
		}
	private:
		SDL_Keycode keycode;
		Type type;
	};

public:
	Keyboard(class Window& wnd);

	bool KeyIsPressed(SDL_Keycode kCode) const;
	Event ReadKey();
	char ReadChar();

	bool KeyIsEmpty() const;

private:
	void OnKeyPressed(SDL_Keycode code);
	void OnKeyReleased(SDL_Keycode code);
	void OnChar(SDL_Keycode code);
	void Flush();
	template<typename T>
	void TrimBuffer(std::queue<T>& buffer);

private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 4u;
	SDL_Event& event;
	std::bitset<nKeys> keystates;
	std::queue<Event> bufferEvents;
	std::queue<char> bufferChar;
};

