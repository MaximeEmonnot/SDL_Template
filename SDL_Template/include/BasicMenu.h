#pragma once
#include "AMenu.h"

class BasicMenu : public AMenu
{
public:
	BasicMenu();

	void Update(int& output, std::shared_ptr<CoreSystem::Mouse> mouse, CoreSystem::Mouse::EventType e = CoreSystem::Mouse::EventType::None) override;
	void Draw(GraphicsEngine::Font& font) override;
};