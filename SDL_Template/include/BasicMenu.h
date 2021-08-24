#pragma once
#include "AMenu.h"

class BasicMenu : public AMenu
{
public:
	BasicMenu();

	void Update(int& output, CoreSystem::Mouse::EventType e = CoreSystem::Mouse::EventType::None) override;
	void Draw() override;
};