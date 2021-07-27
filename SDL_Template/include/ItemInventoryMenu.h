#pragma once

#include "MenuListDecorator.h"
#include "Player.h"

class ItemInventoryMenu : public MenuListDecorator {
public:
	ItemInventoryMenu(std::unique_ptr<AMenu> decoratedMenu);

private:
	std::shared_ptr<Player> pPlayer;
};