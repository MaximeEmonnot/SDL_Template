#include "RightMenu.h"
#include <iostream>

RightMenu::RightMenu(AMenu* decoratedMenu)
	:
	MenuListDecorator(decoratedMenu)
{
	std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void()>>, Maths::IVec2D::Hash> buttonCharacteristics;
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void()>>>(Maths::IVec2D(675, 150), std::make_pair<std::string, std::function<void()>>("Button 1", [] {std::cout << "Button 1!\n"; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void()>>>(Maths::IVec2D(675, 300), std::make_pair<std::string, std::function<void()>>("dflkjmlskdfjmlk", [] {std::cout << "Button 2!\n"; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void()>>>(Maths::IVec2D(675, 450), std::make_pair<std::string, std::function<void()>>("Button 3", [] {std::cout << "Button 3!\n"; })));
	InitializeButtons(buttonCharacteristics);
}