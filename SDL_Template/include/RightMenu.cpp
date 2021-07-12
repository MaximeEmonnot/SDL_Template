#include "RightMenu.h"
#include <iostream>

RightMenu::RightMenu(AMenu* decoratedMenu)
	:
	MenuListDecorator(decoratedMenu)
{
	std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>, Maths::IVec2D::Hash> buttonCharacteristics;
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(675, 400), std::make_pair<std::string, std::function<void(int&)>>("Fight", [](int& out) { std::cout << "Fight!\n"; out = 0; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(675, 450), std::make_pair<std::string, std::function<void(int&)>>("Flee", [](int& out) { std::cout << "Flee!\n"; out = 1; })));
	InitializeButtons(buttonCharacteristics);
}