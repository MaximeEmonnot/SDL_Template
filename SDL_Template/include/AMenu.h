#pragma once
#include "Button.h"
#include "Mouse.h"
#include "Font.h"
#include <vector>

class AMenu {
public:
	AMenu() = default;
	virtual ~AMenu() = default;

	virtual void Update(int& output, CoreSystem::Mouse::EventType e = CoreSystem::Mouse::EventType::None) = 0;
	virtual void Draw() = 0;
};