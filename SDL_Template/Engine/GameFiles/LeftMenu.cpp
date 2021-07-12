#include "LeftMenu.h"
#include <iostream>

LeftMenu::LeftMenu(AMenu* decoratedMenu)
	:
	MenuListDecorator(decoratedMenu)
{
	std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>, Maths::IVec2D::Hash> buttonCharacteristics;
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(10, 125), std::make_pair<std::string, std::function<void(int&)>>("Button 1", [] (int& out) {std::cout << "Button 1!\n"; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(10, 275), std::make_pair<std::string, std::function<void(int&)>>("Button 2", [] (int& out) {std::cout << "Button 2!\n"; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(10, 425), std::make_pair<std::string, std::function<void(int&)>>("Button 3", [] (int& out) {std::cout << "Button 3!\n"; })));
	InitializeButtons(buttonCharacteristics);
}
