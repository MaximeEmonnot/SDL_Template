#pragma once
#include "MenuListDecorator.h"

class RightMenu : public MenuListDecorator
{
public:
	RightMenu(AMenu* decoratedMenu);
};