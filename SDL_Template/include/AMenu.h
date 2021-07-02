#pragma once
#include "Button.h"
#include "Mouse.h"
#include "Font.h"
#include <vector>

class AMenu {
public:
	AMenu() = default;
	virtual void Destroy() = 0;

	virtual void Update(std::shared_ptr<CoreSystem::Mouse> mouse, CoreSystem::Mouse::EventType e = CoreSystem::Mouse::EventType::None) = 0;
	virtual void Draw(GraphicsEngine::Font& font) = 0;
};