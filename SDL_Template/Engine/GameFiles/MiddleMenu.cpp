#include "MiddleMenu.h"
#include <iostream>

MiddleMenu::MiddleMenu(AMenu* decoratedMenu)
	:
	MenuListDecorator(decoratedMenu) 
{
	std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void()>>, Maths::IVec2D::Hash> buttonCharacteristics;
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void()>>>(Maths::IVec2D(250, 200), std::make_pair<std::string, std::function<void()>>("Button 1", [] {std::cout << "Button 1!\n"; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void()>>>(Maths::IVec2D(350, 200), std::make_pair<std::string, std::function<void()>>("Button 2", [] {std::cout << "Button 2!\n"; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void()>>>(Maths::IVec2D(250, 300), std::make_pair<std::string, std::function<void()>>("Button 3", [] {std::cout << "Button 3!\n"; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void()>>>(Maths::IVec2D(350, 300), std::make_pair<std::string, std::function<void()>>("Button 4", [] {std::cout << "Button 4!\n"; })));
	InitializeButtons(buttonCharacteristics);
}