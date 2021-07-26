#pragma once
#include "MenuListDecorator.h"
#include "Player.h"

class PokemonInventory : public MenuListDecorator {
public:
	PokemonInventory(std::unique_ptr<AMenu> decoratedMenu);
private:
	std::shared_ptr<Player> pPlayer;
};