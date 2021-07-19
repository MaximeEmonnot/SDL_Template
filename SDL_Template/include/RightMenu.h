#pragma once
#include "MenuListDecorator.h"

class RightMenu : public MenuListDecorator
{
public:
	RightMenu(std::unique_ptr<AMenu> decoratedMenu);
};