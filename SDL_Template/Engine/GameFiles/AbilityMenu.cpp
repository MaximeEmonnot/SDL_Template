#include "AbilityMenu.h"
#include <iostream>

AbilityMenu::AbilityMenu(std::unique_ptr<AMenu> decoratedMenu)
	:
	MenuListDecorator(std::move(decoratedMenu)),
	pPlayer(Player::GetInstance(Maths::IRect(384, 267, 32, 44), "json/player.json"))
{
	std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>, Maths::IVec2D::Hash> buttonCharacteristics;
	if (pPlayer->GetPokemonList().size() != 0) {
		int i = 0;
		for (auto& attack : pPlayer->GetPokemon().GetAbilities()) {
			buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(675, 400 + i * 15), std::make_pair<std::string, std::function<void(int&)>>(attack.GetName() + "   " + std::to_string(attack.GetPP()) + " / " + std::to_string(attack.GetMaxPP()) + "PP", [attackName = attack.GetName(), id = i](int& out) {std::cout << attackName << "!\n"; out = id; })));
			i++;
		}
	}
	buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(700, 575), std::make_pair<std::string, std::function<void(int&)>>("Back", [&](int& out) {std::cout << "Back!\n"; out = 30; })));
	InitializeButtons(buttonCharacteristics);
}
