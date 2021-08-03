#pragma once
#include "MenuListDecorator.h"
#include "Player.h"

class PokemonMenu : public MenuListDecorator {
public :
	PokemonMenu(std::unique_ptr<AMenu> decoratedMenu);
private:
	std::shared_ptr<Player> pPlayer;
};