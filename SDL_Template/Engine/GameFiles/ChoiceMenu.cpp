#include "ChoiceMenu.h"
#include <iostream>

ChoiceMenu::ChoiceMenu(std::unique_ptr<AMenu> decoratedMenu)
	:
	MenuListDecorator(std::move(decoratedMenu))
{
	std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>, Maths::IVec2D::Hash> buttonCharacteristics;
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(200, 525), std::make_pair<std::string, std::function<void(int&)>>("Bulbasaur", [](int& out) {std::cout << "Bulbasaur!\n"; out = 1; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(400, 525), std::make_pair<std::string, std::function<void(int&)>>("Charmander", [](int& out) {std::cout << "Charmander!\n"; out = 2; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(600, 525), std::make_pair<std::string, std::function<void(int&)>>("Squirttle", [](int& out) {std::cout << "Squirttle!\n"; out = 3; })));
	InitializeButtons(buttonCharacteristics);
}