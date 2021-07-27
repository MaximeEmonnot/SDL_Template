#pragma once
#include "MenuListDecorator.h"

class ExplorationMenu : public MenuListDecorator {
public:
	ExplorationMenu(std::unique_ptr<AMenu> decoratedMenu);
};