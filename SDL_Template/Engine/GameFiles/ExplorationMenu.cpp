#include "ExplorationMenu.h"
#include <iostream>

ExplorationMenu::ExplorationMenu(std::unique_ptr<AMenu> decoratedMenu)
	:
	MenuListDecorator(std::move(decoratedMenu))
{
	std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>, Maths::IVec2D::Hash> buttonCharacteristics;
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(700, 150), std::make_pair<std::string, std::function<void(int&)>>("Items", [](int& out) {std::cout << "Items!\n"; out = 1; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(700, 200), std::make_pair<std::string, std::function<void(int&)>>("Pokemons", [](int& out) {std::cout << "Pokemons!\n"; out = 2; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(700, 250), std::make_pair<std::string, std::function<void(int&)>>("Bike", [](int& out) {std::cout << "Bike!\n"; out = 3; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(700, 300), std::make_pair<std::string, std::function<void(int&)>>("Start Server", [](int& out) {std::cout << "Server!\n"; out = 4; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(700, 350), std::make_pair<std::string, std::function<void(int&)>>("Save game", [](int& out) {std::cout << "Saving the game!\n"; out = 5; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(700, 400), std::make_pair<std::string, std::function<void(int&)>>("Exit to title screen", [](int& out) {std::cout << "Going to title screen!\n"; out = 6; })));
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(700, 450), std::make_pair<std::string, std::function<void(int&)>>("Exit game", [](int& out) {std::cout << "Exiting the game!\n"; out = 0; })));
	InitializeButtons(buttonCharacteristics);
}