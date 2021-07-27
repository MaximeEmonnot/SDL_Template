#pragma once
#include "MenuListDecorator.h"

class ChoiceMenu : public MenuListDecorator
{
public:
	ChoiceMenu(std::unique_ptr<AMenu> decoratedMenu);
};