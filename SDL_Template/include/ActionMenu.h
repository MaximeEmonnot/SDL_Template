#pragma once
#include "MenuListDecorator.h"

class ActionMenu : public MenuListDecorator
{
public:
	ActionMenu(std::unique_ptr<AMenu> decoratedMenu);
};