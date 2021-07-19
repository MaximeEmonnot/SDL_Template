#pragma once
#include "MenuListDecorator.h"

class LeftMenu : public MenuListDecorator {
public:
	LeftMenu(std::unique_ptr<AMenu> decoratedMenu);
};