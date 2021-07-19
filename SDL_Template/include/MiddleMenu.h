#pragma once
#include "MenuListDecorator.h"

class MiddleMenu : public MenuListDecorator
{
public:
	MiddleMenu(std::unique_ptr<AMenu> decoratedMenu);
};