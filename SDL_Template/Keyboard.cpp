#define CORSAIR_LIGHTING_SDK_DISABLE_DEPRECATION_WARNINGS

#include "Keyboard.h"
#include "Graphics.h"
#include <thread>
#include <algorithm>

Keyboard::Keyboard()
{
	CorsairPerformProtocolHandshake();
	CorsairRequestControl(CAM_ExclusiveLightingControl);
	auto leds = CorsairGetLedPositions();
	double maxX = 0.0, maxY = 0.0;
	for (int i = 0; i < leds->numberOfLed; i++) {
		auto pTemp = leds->pLedPosition[i];
		if (pTemp.left + pTemp.width > maxX) maxX = pTemp.left + pTemp.width;
		if (pTemp.top + pTemp.height > maxY) maxY = pTemp.top + pTemp.height;
		SDL_Rect rTemp = { pTemp.left, pTemp.top, pTemp.width, pTemp.height };
		rectKeys.push_back(rTemp);
		ledPositions.insert(std::pair<SDL_Rect,CorsairLedId>(rTemp, pTemp.ledId));
	}
	xFactor = Graphics::width / maxX;
	yFactor = Graphics::height / maxY;
}

bool Keyboard::KeyIsPressed(SDL_Keycode kCode) const
{
	return keystates[SDL_GetScancodeFromKey(kCode)];
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

void Keyboard::FlushKeyColors()
{
	for (auto& entry : ledPositions) {
		auto ledColor = CorsairLedColor{ entry.second, 0, 0, 0 };
		CorsairSetLedsColors(1, &ledColor);
	}
}

void Keyboard::SetKeyColor(SDL_Scancode kCode, SDL_Color c)
{
	auto changeColor = [&]() { 	auto ledColor = CorsairLedColor{ SDLKeyToCorsairId(kCode), c.r, c.g, c.b };
	CorsairSetLedsColors(1, &ledColor);
	};
	std::thread t1(changeColor);
	t1.join();
}

void Keyboard::SetKeyColorByPosition(IVec2D pos, SDL_Color c)
{
	pos.x /= xFactor;
	pos.y /= yFactor;
	auto itr = std::find_if(ledPositions.begin(), ledPositions.end(), [&](const auto& pair) { return RectContainsVec(pair.first, pos); });
	if (itr != ledPositions.end()) {
		auto ledColor = CorsairLedColor{ itr->second, c.r, c.g, c.b };
		CorsairSetLedsColors(1, &ledColor);
	}
}

void Keyboard::SetKeyColorByRect(SDL_Rect rect, SDL_Color c)
{
	auto itr = ledPositions.find(rect);
	auto ledColor = CorsairLedColor{ itr->second, c.r, c.g, c.b };
	CorsairSetLedsColors(1, &ledColor);
}

void Keyboard::FadeKeyColorTo(SDL_Rect rect, SDL_Color c, float alpha)
{
	auto itr = ledPositions.find(rect);
	auto currentLedColor = CorsairLedColor{ itr->second, 0, 0, 0 };
	CorsairGetLedsColors(1, &currentLedColor);
	SDL_Color newColor = BlendColor(SDL_Color({ (Uint8)currentLedColor.r, (Uint8)currentLedColor.g, (Uint8)currentLedColor.b, 255 }), c, alpha);
	auto newLedColor = CorsairLedColor{ itr->second, newColor.r, newColor.g, newColor.b };
	CorsairSetLedsColors(1, &newLedColor);
}

SDL_Color Keyboard::GetKeyColorByPosition(SDL_Rect rect)
{
	auto itr = ledPositions.find(rect);
	SDL_Color output = { 0, 0, 0, 255 };
	auto ledColor = CorsairLedColor{ itr->second, 0, 0, 0 };
	CorsairGetLedsColors(1, &ledColor);
	output.r = ledColor.r;
	output.g = ledColor.g;
	output.b = ledColor.b;
	return output;
}

SDL_Rect Keyboard::GetKeyRect(SDL_Scancode kCode)
{
	auto itr = std::find_if(ledPositions.begin(), ledPositions.end(), [&](const auto& pair) { return pair.second == SDLKeyToCorsairId(kCode); });
	return itr->first;
}

std::vector<SDL_Rect> Keyboard::GetKeyboardRect() const
{
	return rectKeys;
}

void Keyboard::OnKeyPressed(SDL_Scancode code)
{
	keystates[SDL_GetScancodeFromKey(code)] = true;
	bufferEvents.push(Event(code, Event::Type::Pressed));
	TrimBuffer(bufferEvents);
}

void Keyboard::OnKeyReleased(SDL_Scancode code)
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

CorsairLedId Keyboard::SDLKeyToCorsairId(SDL_Scancode kCode)
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
void Keyboard::TrimBuffer(std::queue<T>& buffer)
{
	if (buffer.size() > bufferSize) {
		buffer.pop();
	}
}
