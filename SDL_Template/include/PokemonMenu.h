#pragma once
#include "MenuListDecorator.h"
#include "Pokemon.h"

class PokemonMenu : public MenuListDecorator {
public:
	PokemonMenu(AMenu* decoratedMenu, const Pokemon& pkmn);
};