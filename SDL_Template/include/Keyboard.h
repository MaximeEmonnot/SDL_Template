#pragma once
#include <CUESDK.h>
#include <SDL_keyboard.h>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <queue>
#include "Rect.h"
#include "DColors.h"
#include "SingletonMaker.h"

namespace CoreSystem {
	class Keyboard : public SingletonMaker<CoreSystem::Keyboard>
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


		//CORSAIR

		void                            FlushKeyColors();
		void                            SetKeyboardColor(GraphicsEngine::Color c);
		void							SetKeyColor(SDL_Scancode kCode, GraphicsEngine::Color c);
		void                            SetKeyColorByPosition(Maths::IVec2D pos, GraphicsEngine::Color c);
		void                            SetKeyColorByRect(Maths::IRect rect, GraphicsEngine::Color c);
		void                            FadeKeyColorTo(Maths::IRect rect, GraphicsEngine::Color c, float alpha = .5f);

		GraphicsEngine::Color           GetKeyColorByPosition(Maths::IRect rect);
		Maths::IRect                    GetKeyRect(SDL_Scancode kCode);
		std::vector<Maths::IRect>       GetKeyboardRect() const;
		CorsairLedId                    GetLedIdFrom(Maths::IRect rect) const;

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

		//CORSAIR

		CorsairLedId					SDLKeyToCorsairId(SDL_Scancode kCode);

	private:
		const unsigned int													mBufferSize = 4u;
		std::bitset<SDL_NUM_SCANCODES>										mKeystates;
		std::queue<Event>													mBufferEvents;
		std::queue<char>													mBufferChar;

		//CORSAIR
		std::unordered_map<Maths::IRect, CorsairLedId, Maths::IRect::Hash>	mLedPositions;
		std::vector<Maths::IRect>											mRectKeys;
		bool																mbIsCorsairKeyboard;
		int																	mKeyboardWidth;
		int																	mKeyboardHeight;
	};
}