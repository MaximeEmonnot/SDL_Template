#include "ActionMenu.h"
#include <iostream>

ActionMenu::ActionMenu(std::unique_ptr<AMenu> decoratedMenu)
	:
	MenuListDecorator(std::move(decoratedMenu))
{
	std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>, Maths::IVec2D::Hash> buttonCharacteristics;
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(625, 400), std::make_pair<std::string, std::function<void(int&)>>("Fight", [](int& out) { std::cout << "Fight!\n"; out = 0; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(625, 450), std::make_pair<std::string, std::function<void(int&)>>("Items", [](int& out) { std::cout << "Items!\n"; out = 1; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(625, 500), std::make_pair<std::string, std::function<void(int&)>>("Pokemons", [](int& out) { std::cout << "Pokemons!\n"; out = 2; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(625, 550), std::make_pair<std::string, std::function<void(int&)>>("Flee", [](int& out) { std::cout << "Flee!\n"; out = 3; })));
	InitializeButtons(buttonCharacteristics);
}