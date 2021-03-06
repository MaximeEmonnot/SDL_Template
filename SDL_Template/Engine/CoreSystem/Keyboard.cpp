#define CORSAIR_LIGHTING_SDK_DISABLE_DEPRECATION_WARNINGS

#include "Keyboard.h"
#include "Graphics.h"
#include <thread>
#include <algorithm>

CoreSystem::Keyboard::Keyboard()
{
	CorsairPerformProtocolHandshake();
	if (const auto error = CorsairGetLastError()) {
		mbIsCorsairKeyboard = false;
		mKeyboardHeight = mKeyboardWidth = 0;
	}
	else {
		CorsairRequestControl(CAM_ExclusiveLightingControl);
		mbIsCorsairKeyboard = true;
		auto leds = CorsairGetLedPositions();
		double maxX = 0.0, maxY = 0.0;
		for (int i = 0; i < leds->numberOfLed; i++) {
			auto pTemp = leds->pLedPosition[i];
			if (pTemp.left + pTemp.width > maxX) maxX = pTemp.left + pTemp.width;
			if (pTemp.top + pTemp.height > maxY) maxY = pTemp.top + pTemp.height;
			Maths::IRect rTemp = { (int)pTemp.left, (int)pTemp.top, (int)pTemp.width, (int)pTemp.height };
			mRectKeys.push_back(rTemp);
			mLedPositions.insert(std::pair<Maths::IRect, CorsairLedId>(rTemp, pTemp.ledId));
		}
		mKeyboardWidth = (int)maxX, mKeyboardHeight = (int)maxY;
	}
}

CoreSystem::Keyboard::~Keyboard()
{
	CorsairReleaseControl(CAM_ExclusiveLightingControl);
	Flush();
}

bool CoreSystem::Keyboard::KeyIsPressed(SDL_Scancode kCode) const
{
	return mKeystates[kCode];
}

CoreSystem::Keyboard::Event CoreSystem::Keyboard::ReadKey()
{
	if (mBufferEvents.size() > 0u) {
		Keyboard::Event e = mBufferEvents.front();
		mBufferEvents.pop();
		return e;
	}
	return Keyboard::Event();
}

char CoreSystem::Keyboard::ReadChar()
{
	if (mBufferChar.size() > 0u) {
		char c = mBufferChar.front();
		mBufferChar.pop();
		return c;
	}
	return 0;
}

bool CoreSystem::Keyboard::KeyIsEmpty() const
{
	return mBufferEvents.empty();
}

void CoreSystem::Keyboard::FlushKeyColors()
{
	for (auto& entry : mLedPositions) {
		auto ledColor = CorsairLedColor{ entry.second, 0, 0, 0 };
		CorsairSetLedsColors(1, &ledColor);
	}
}

void CoreSystem::Keyboard::SetKeyboardColor(GraphicsEngine::Color c)
{
	for (auto& led : mLedPositions) {
		auto ledColor = CorsairLedColor{ led.second, c.c.r, c.c.g, c.c.b };
		CorsairSetLedsColors(1, &ledColor);
	}
}

void CoreSystem::Keyboard::SetKeyColor(SDL_Scancode kCode, GraphicsEngine::Color c)
{
	auto ledColor = CorsairLedColor{ SDLKeyToCorsairId(kCode), c.c.r, c.c.g, c.c.b };
	CorsairSetLedsColors(1, &ledColor);
}

void CoreSystem::Keyboard::SetKeyColorByPosition(Maths::IVec2D pos, GraphicsEngine::Color c)
{

	auto itr = std::find_if(mLedPositions.begin(), mLedPositions.end(), [&](const auto& pair) { return pair.first.ContainsVec2D(pos); });
	if (itr != mLedPositions.end()) {
		auto ledColor = CorsairLedColor{ itr->second, c.c.r, c.c.g, c.c.b };
		CorsairSetLedsColors(1, &ledColor);
	}
}

void CoreSystem::Keyboard::SetKeyColorByRect(Maths::IRect rect, GraphicsEngine::Color c)
{
	auto itr = mLedPositions.find(rect);
	auto ledColor = CorsairLedColor{ itr->second, c.c.r, c.c.g, c.c.b };
	CorsairSetLedsColors(1, &ledColor);
}

void CoreSystem::Keyboard::FadeKeyColorTo(Maths::IRect rect, GraphicsEngine::Color c, float alpha)
{
	auto itr = mLedPositions.find(rect);
	auto currentLedColor = CorsairLedColor{ itr->second, 0, 0, 0 };
	CorsairGetLedsColors(1, &currentLedColor);
	GraphicsEngine::Color newColor = GraphicsEngine::Color((Uint8)currentLedColor.r, (Uint8)currentLedColor.g, (Uint8)currentLedColor.b, 255).BlendColor(GraphicsEngine::Color(c.c.r, c.c.g, c.c.b, c.c.a), alpha);
	auto newLedColor = CorsairLedColor{ itr->second, newColor.c.r, newColor.c.g, newColor.c.b };
	CorsairSetLedsColors(1, &newLedColor);
}

GraphicsEngine::Color CoreSystem::Keyboard::GetKeyColorByPosition(Maths::IRect rect)
{
	auto itr = mLedPositions.find(rect);
	GraphicsEngine::Color output = { 0, 0, 0, 255 };
	auto ledColor = CorsairLedColor{ itr->second, 0, 0, 0 };
	CorsairGetLedsColors(1, &ledColor);
	output.c.r = ledColor.r;
	output.c.g = ledColor.g;
	output.c.b = ledColor.b;
	return output;
}

Maths::IRect CoreSystem::Keyboard::GetKeyRect(SDL_Scancode kCode)
{
	auto itr = std::find_if(mLedPositions.begin(), mLedPositions.end(), [&](const auto& pair) { return pair.second == SDLKeyToCorsairId(kCode); });
	return itr->first;
}

std::vector<Maths::IRect> CoreSystem::Keyboard::GetKeyboardRect() const
{
	return mRectKeys;
}

CorsairLedId CoreSystem::Keyboard::GetLedIdFrom(Maths::IRect rect) const
{
	return mLedPositions.find(rect)->second;
}

bool CoreSystem::Keyboard::IsCorsairKeyboard() const
{
	return mbIsCorsairKeyboard;
}

int CoreSystem::Keyboard::GetKeyboardWidth() const
{
	return mKeyboardWidth;
}

int CoreSystem::Keyboard::GetKeyboardHeight() const
{
	return mKeyboardHeight;
}

void CoreSystem::Keyboard::OnKeyPressed(SDL_Scancode code)
{
	mKeystates[code] = true;
	mBufferEvents.push({ code, Event::Type::Pressed });
	TrimBuffer(mBufferEvents);
}

void CoreSystem::Keyboard::OnKeyReleased(SDL_Scancode code)
{
	mKeystates[code] = false;
	mBufferEvents.push({ code, Event::Type::Released });
	TrimBuffer(mBufferEvents);
}

void CoreSystem::Keyboard::OnChar(SDL_Keycode code)
{
	mBufferChar.push(code);
	TrimBuffer(mBufferChar);
}

void CoreSystem::Keyboard::Flush()
{
	mKeystates.reset();
	mBufferEvents = std::queue<Event>();
	mBufferChar = std::queue<char>();
}

CorsairLedId CoreSystem::Keyboard::SDLKeyToCorsairId(SDL_Scancode kCode)
{
	switch (kCode) {
	case SDL_SCANCODE_UNKNOWN:
		return CLI_Invalid;
		break;
	case SDL_SCANCODE_ESCAPE:
		return CLK_Escape;
		break;
	case SDL_SCANCODE_F1:
		return CLK_F1;
		break;
	case SDL_SCANCODE_F2:
		return CLK_F2;
		break;
	case SDL_SCANCODE_F3:
		return CLK_F3;
		break;
	case SDL_SCANCODE_F4:
		return CLK_F4;
		break;
	case SDL_SCANCODE_F5:
		return CLK_F5;
		break;
	case SDL_SCANCODE_F6:
		return CLK_F6;
		break;
	case SDL_SCANCODE_F7:
		return CLK_F7;
		break;
	case SDL_SCANCODE_F8:
		return CLK_F8;
		break;
	case SDL_SCANCODE_F9:
		return CLK_F9;
		break;
	case SDL_SCANCODE_F10:
		return CLK_F10;
		break;
	case SDL_SCANCODE_F11:
		return CLK_F11;
		break;
	case SDL_SCANCODE_F12:
		return CLK_F12;
		break;
	case SDL_SCANCODE_PRINTSCREEN:
		return CLK_PrintScreen;
		break;
	case SDL_SCANCODE_SCROLLLOCK:
		return CLK_ScrollLock;
		break;
	case SDL_SCANCODE_PAUSE:
		return CLK_PauseBreak;
		break;
	case SDL_SCANCODE_GRAVE:
		return CLK_GraveAccentAndTilde;
		break;
	case SDL_SCANCODE_1:
		return CLK_1;
		break;
	case SDL_SCANCODE_2:
		return CLK_2;
		break;
	case SDL_SCANCODE_3:
		return CLK_3;
		break;
	case SDL_SCANCODE_4:
		return CLK_4;
		break;
	case SDL_SCANCODE_5:
		return CLK_5;
		break;
	case SDL_SCANCODE_6:
		return CLK_6;
		break;
	case SDL_SCANCODE_7:
		return CLK_7;
		break;
	case SDL_SCANCODE_8:
		return CLK_8;
		break;
	case SDL_SCANCODE_9:
		return CLK_9;
		break;
	case SDL_SCANCODE_0:
		return CLK_0;
		break;
	case SDL_SCANCODE_MINUS:
		return CLK_MinusAndUnderscore;
		break;
	case SDL_SCANCODE_EQUALS:
		return CLK_EqualsAndPlus;
		break;
	case SDL_SCANCODE_BACKSPACE:
		return CLK_Backspace;
		break;
	case SDL_SCANCODE_TAB:
		return CLK_Tab;
		break;
	case SDL_SCANCODE_Q:
		return CLK_Q;
		break;
	case SDL_SCANCODE_W:
		return CLK_W;
		break;
	case SDL_SCANCODE_E:
		return CLK_E;
		break;
	case SDL_SCANCODE_R:
		return CLK_R;
		break;
	case SDL_SCANCODE_T:
		return CLK_T;
		break;
	case SDL_SCANCODE_Y:
		return CLK_Y;
		break;
	case SDL_SCANCODE_U:
		return CLK_U;
		break;
	case SDL_SCANCODE_I:
		return CLK_I;
		break;
	case SDL_SCANCODE_O:
		return CLK_O;
		break;
	case SDL_SCANCODE_P:
		return CLK_P;
		break;
	case SDL_SCANCODE_LEFTBRACKET:
		return CLK_BracketLeft;
		break;
	case SDL_SCANCODE_RIGHTBRACKET:
		return CLK_BracketRight;
		break;
	case SDL_SCANCODE_RETURN:
		return CLK_Enter;
		break;
	case SDL_SCANCODE_CAPSLOCK:
		return CLK_CapsLock;
		break;
	case SDL_SCANCODE_A:
		return CLK_A;
		break;
	case SDL_SCANCODE_S:
		return CLK_S;
		break;
	case SDL_SCANCODE_D:
		return CLK_D;
		break;
	case SDL_SCANCODE_F:
		return CLK_F;
		break;
	case SDL_SCANCODE_G:
		return CLK_G;
		break;
	case SDL_SCANCODE_H:
		return CLK_H;
		break;
	case SDL_SCANCODE_J:
		return CLK_J;
		break;
	case SDL_SCANCODE_K:
		return CLK_K;
		break;
	case SDL_SCANCODE_L:
		return CLK_L;
		break;
	case SDL_SCANCODE_SEMICOLON:
		return CLK_SemicolonAndColon;
		break;
	case SDL_SCANCODE_APOSTROPHE:
		return CLK_ApostropheAndDoubleQuote;
		break;
	case SDL_SCANCODE_BACKSLASH:;
		return CLK_Backslash;
		break;
	case SDL_SCANCODE_LSHIFT:
		return CLK_LeftShift;
		break;
	case SDL_SCANCODE_NONUSBACKSLASH:
		return CLK_NonUsBackslash;
		break;
	case SDL_SCANCODE_Z:
		return CLK_Z;
		break;
	case SDL_SCANCODE_X:
		return CLK_X;
		break;
	case SDL_SCANCODE_C:
		return CLK_C;
		break;
	case SDL_SCANCODE_V:
		return CLK_V;
		break;
	case SDL_SCANCODE_B:
		return CLK_B;
		break;
	case SDL_SCANCODE_N:
		return CLK_N;
		break;
	case SDL_SCANCODE_M:
		return CLK_M;
		break;
	case SDL_SCANCODE_COMMA:
		return CLK_CommaAndLessThan;
		break;
	case SDL_SCANCODE_PERIOD:
		return CLK_PeriodAndBiggerThan;
		break;
	case SDL_SCANCODE_SLASH:
		return CLK_SlashAndQuestionMark;
		break;
	case SDL_SCANCODE_RSHIFT:
		return CLK_RightShift;
		break;
	case SDL_SCANCODE_LCTRL:
		return CLK_LeftCtrl;
		break;
	case SDL_SCANCODE_LGUI:
		return CLK_LeftGui;
		break;
	case SDL_SCANCODE_LALT:
		return CLK_LeftAlt;
		break;
	case SDL_SCANCODE_SPACE:
		return CLK_Space;
		break;
	case SDL_SCANCODE_RALT:
		return CLK_RightAlt;
		break;
	case SDL_SCANCODE_RGUI:
		return CLK_RightGui;
		break;
	case SDL_SCANCODE_APPLICATION:
		return CLK_Application;
		break;
	case SDL_SCANCODE_RCTRL:
		return CLK_RightCtrl;
		break;
	case SDL_SCANCODE_INSERT:
		return CLK_Insert;
		break;
	case SDL_SCANCODE_HOME:
		return CLK_Home;
		break;
	case SDL_SCANCODE_PAGEUP:
		return CLK_PageUp;
		break;
	case SDL_SCANCODE_DELETE:
		return CLK_Delete;
		break;
	case SDL_SCANCODE_END:
		return CLK_End;
		break;
	case SDL_SCANCODE_PAGEDOWN:
		return CLK_PageDown;
		break;
	case SDL_SCANCODE_UP:
		return CLK_UpArrow;
		break;
	case SDL_SCANCODE_LEFT:
		return CLK_LeftArrow;
		break;
	case SDL_SCANCODE_DOWN:
		return CLK_DownArrow;
		break;
	case SDL_SCANCODE_RIGHT:
		return CLK_RightArrow;
		break;
	case SDL_SCANCODE_NUMLOCKCLEAR:
		return CLK_NumLock;
		break;
	case SDL_SCANCODE_KP_0:
		return CLK_Keypad0;
		break;
	case SDL_SCANCODE_KP_1:
		return CLK_Keypad1;
		break;
	case SDL_SCANCODE_KP_2:
		return CLK_Keypad2;
		break;
	case SDL_SCANCODE_KP_3:
		return CLK_Keypad3;
		break;
	case SDL_SCANCODE_KP_4:
		return CLK_Keypad4;
		break;
	case SDL_SCANCODE_KP_5:
		return CLK_Keypad5;
		break;
	case SDL_SCANCODE_KP_6:
		return CLK_Keypad6;
		break;
	case SDL_SCANCODE_KP_7:
		return CLK_Keypad7;
		break;
	case SDL_SCANCODE_KP_8:
		return CLK_Keypad8;
		break;
	case SDL_SCANCODE_KP_9:
		return CLK_Keypad9;
		break;
	case SDL_SCANCODE_KP_PERIOD:
		return CLK_KeypadPeriodAndDelete;
		break;
	case SDL_SCANCODE_KP_PLUS:
		return CLK_KeypadPlus;
		break;
	case SDL_SCANCODE_KP_MINUS:
		return CLK_KeypadMinus;
		break;
	case SDL_SCANCODE_KP_MULTIPLY:
		return CLK_KeypadAsterisk;
		break;
	case SDL_SCANCODE_KP_DIVIDE:
		return CLK_KeypadSlash;
		break;
	case SDL_SCANCODE_KP_ENTER:
		return CLK_KeypadEnter;
		break;
	default:
		return CLI_Invalid;
		break;
	}
}

template <typename T>
void CoreSystem::Keyboard::TrimBuffer(std::queue<T>& buffer)
{
	if (buffer.size() > mBufferSize) {
		buffer.pop();
	}
}
