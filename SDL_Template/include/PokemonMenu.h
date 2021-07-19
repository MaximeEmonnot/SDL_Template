#pragma once
#include "MenuListDecorator.h"
#include "Pokemon.h"

class PokemonMenu : public MenuListDecorator {
public:
	PokemonMenu(std::unique_ptr<AMenu> decoratedMenu, const Pokemon& pkmn);
};