#include "MiddleMenu.h"
#include <iostream>

MiddleMenu::MiddleMenu(std::unique_ptr<AMenu> decoratedMenu)
	:
	MenuListDecorator(std::move(decoratedMenu)) 
{
	std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>, Maths::IVec2D::Hash> buttonCharacteristics;
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(300, 300), std::make_pair<std::string, std::function<void(int&)>>("Play", [&](int& out) {std::cout << "Play!\n"; out = 1; })));
	
	//Options
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(400, 300), std::make_pair<std::string, std::function<void(int&)>>("Delete file", [&](int& out) {std::cout << "File deleted!\n"; out = 2; })));
	//Exit
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(500, 300), std::make_pair<std::string, std::function<void(int&)>>("Exit", [&](int& out) {std::cout << "Exit!\n"; out = 0; })));
	
	InitializeButtons(buttonCharacteristics);
}