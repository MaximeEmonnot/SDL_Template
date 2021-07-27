#pragma once
#include "MenuListDecorator.h"
#include "Player.h"

class AbilityMenu : public MenuListDecorator {
public:
	AbilityMenu(std::unique_ptr<AMenu> decoratedMenu);

private:
	std::shared_ptr<Player> pPlayer;
};

class PokemonMenu : public MenuListDecorator {
public :
	PokemonMenu(std::unique_ptr<AMenu> decoratedMenu);
private:
	std::shared_ptr<Player> pPlayer;
};