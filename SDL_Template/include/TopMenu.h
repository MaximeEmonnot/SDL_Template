#pragma once
#include "MenuListDecorator.h"

class TopMenu : public MenuListDecorator {
public:
	TopMenu(std::unique_ptr<AMenu> decoratedMenu);
};