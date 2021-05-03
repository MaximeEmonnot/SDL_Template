#pragma once
#include <CUESDK.h>
#include <SDL_keyboard.h>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <queue>
#include "Vec2D.h"

namespace std {
	template<>
	struct hash<SDL_Rect>
	{
		std::size_t operator()(const SDL_Rect& r) const {
			using std::size_t;
			using std::hash;
			return ((hash<int>()(r.x) ^ (hash<int>()(r.y) << 1)) >> 1) ^ ((hash<int>()(r.w) ^ hash<int>()(r.h) << 1) >> 1);
		}
	};
	template<>
	struct equal_to<SDL_Rect>
	{
		bool operator() (const SDL_Rect& lhs, const SDL_Rect& rhs) const{
			return lhs.x == rhs.x && lhs.y == rhs.y && lhs.h == rhs.h && lhs.w == rhs.w;
		}
	};
}

namespace CoreSystem {
	class Keyboard
	{
	private:
		friend class Window;
	public:
		struct Event {
			enum class Type {
				Pressed,
				Released,
				None
			};
			SDL_Scancode keycode = SDL_SCANCODE_UNKNOWN;
			Type type = Type::None;
		};

	public:
		Keyboard();
		~Keyboard();

		// Given a keycode, returns true if the key is pressed, false otherwise.
		bool							KeyIsPressed(SDL_Scancode kCode) const;
		// Returns the last key pressed.
		Event							ReadKey();
		// Returns the last character registered.
		char							ReadChar();
		// Returns true if no keys are pressed, false otherwise.
		bool							KeyIsEmpty() const;

		void                            FlushKeyColors();
		void                            SetKeyboardColor(SDL_Color c);
		void							SetKeyColor(SDL_Scancode kCode, SDL_Color c);
		void                            SetKeyColorByPosition(IVec2D pos, SDL_Color c);
		void                            SetKeyColorByRect(SDL_Rect rect, SDL_Color c);
		void                            FadeKeyColorTo(SDL_Rect rect, SDL_Color c, float alpha = .5f);

		SDL_Color                       GetKeyColorByPosition(SDL_Rect rect);
		SDL_Rect                        GetKeyRect(SDL_Scancode kCode);
		std::vector<SDL_Rect>           GetKeyboardRect() const;
		CorsairLedId                    GetLedIdFrom(SDL_Rect rect) const;

		bool                            IsCorsairKeyboard() const;
		int								GetKeyboardWidth() const;
		int								GetKeyboardHeight() const;

	private:
		void							OnKeyPressed(SDL_Scancode code);
		void							OnKeyReleased(SDL_Scancode code);
		void							OnChar(SDL_Keycode code);
		void							Flush();
		template<typename T>
		void							TrimBuffer(std::queue<T>& buffer);

		CorsairLedId					SDLKeyToCorsairId(SDL_Scancode kCode);

	private:
		static constexpr unsigned int	nKeys = SDL_NUM_SCANCODES;
		static constexpr unsigned int	bufferSize = 4u;
		std::bitset<nKeys>				keystates;
		std::queue<Event>				bufferEvents;
		std::queue<char>				bufferChar;
		std::unordered_map<SDL_Rect, CorsairLedId> ledPositions;
		std::vector<SDL_Rect>           rectKeys;
		bool                            isCorsairKeyboard;
		int								keyboardWidth;
		int								keyboardHeight;
	};
}