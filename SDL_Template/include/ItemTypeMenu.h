#pragma once
#include "MenuListDecorator.h"

class ItemTypeMenu : public MenuListDecorator {
public:
	ItemTypeMenu(std::unique_ptr<AMenu> decoratedMenu);
};