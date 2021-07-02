#pragma once
#include "MenuListDecorator.h"

class BottomMenu : public MenuListDecorator
{
public:
	BottomMenu(AMenu* decoratedMenu);
};