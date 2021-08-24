#pragma once
#include "MenuListDecorator.h"
#include "Player.h"

class PokemonInventoryMenu : public MenuListDecorator {
public:
	PokemonInventoryMenu(std::unique_ptr<AMenu> decoratedMenu, std::shared_ptr<Player> pPlayer);
};