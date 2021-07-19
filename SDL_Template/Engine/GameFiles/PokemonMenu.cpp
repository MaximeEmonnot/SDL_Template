#include "PokemonMenu.h"
#include <iostream>

PokemonMenu::PokemonMenu(std::unique_ptr<AMenu> decoratedMenu, const Pokemon& pkmn)
	:
	MenuListDecorator(std::move(decoratedMenu))
{
	std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>, Maths::IVec2D::Hash> buttonCharacteristics;
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(675, 400), std::make_pair<std::string, std::function<void(int&)>>(pkmn.GetAbility(0).name, [&](int& out) {std::cout << pkmn.GetAbility(0).name << "!\n"; out = 0; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(675, 450), std::make_pair<std::string, std::function<void(int&)>>(pkmn.GetAbility(1).name, [&](int& out) {std::cout << pkmn.GetAbility(1).name << "!\n"; out = 1; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(675, 500), std::make_pair<std::string, std::function<void(int&)>>(pkmn.GetAbility(2).name, [&](int& out) {std::cout << pkmn.GetAbility(2).name << "!\n"; out = 2; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(675, 550), std::make_pair<std::string, std::function<void(int&)>>(pkmn.GetAbility(3).name, [&](int& out) {std::cout << pkmn.GetAbility(3).name << "!\n"; out = 3; })));
	InitializeButtons(buttonCharacteristics);
}