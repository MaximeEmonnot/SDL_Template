#pragma once
#include "MenuListDecorator.h"

class MiddleMenu : public MenuListDecorator
{
public:
	MiddleMenu(AMenu* decoratedMenu);
};