#pragma once
#include "MenuListDecorator.h"

class LeftMenu : public MenuListDecorator {
public:
	LeftMenu(AMenu* decoratedMenu);
};