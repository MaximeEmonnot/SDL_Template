#pragma once
#include "MenuListDecorator.h"

class TopMenu : public MenuListDecorator {
public:
	TopMenu(AMenu* decoratedMenu);
};