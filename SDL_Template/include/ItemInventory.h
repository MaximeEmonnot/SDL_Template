#pragma once

#include "MenuListDecorator.h"
#include "Player.h"

class ItemInventory : public MenuListDecorator {
public:
	ItemInventory(std::unique_ptr<AMenu> decoratedMenu);

private:
	std::shared_ptr<Player> pPlayer;
};