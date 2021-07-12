#include "BottomMenu.h"
#include <iostream>

BottomMenu::BottomMenu(AMenu* decoratedMenu)
	:
	MenuListDecorator(decoratedMenu)
{
	std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>, Maths::IVec2D::Hash> buttonCharacteristics;
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(200, 525), std::make_pair<std::string, std::function<void(int&)>>("Button 1", [](int& out){std::cout << "Button 1!\n"; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(400, 525), std::make_pair<std::string, std::function<void(int&)>>("Button 2", [](int& out){std::cout << "Button 2!\n"; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(600, 525), std::make_pair<std::string, std::function<void(int&)>>("Button 3", [](int& out){std::cout << "Button 3!\n"; })));
	InitializeButtons(buttonCharacteristics);
}
