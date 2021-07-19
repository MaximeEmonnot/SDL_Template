#pragma once
#include "MenuListDecorator.h"

class BottomMenu : public MenuListDecorator
{
public:
	BottomMenu(std::unique_ptr<AMenu> decoratedMenu);
};