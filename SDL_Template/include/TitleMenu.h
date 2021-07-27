#pragma once
#include "MenuListDecorator.h"

class TitleMenu : public MenuListDecorator
{
public:
	TitleMenu(std::unique_ptr<AMenu> decoratedMenu);
};