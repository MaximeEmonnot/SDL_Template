#include "PokemonInventoryMenu.h"

PokemonInventoryMenu::PokemonInventoryMenu(std::unique_ptr<AMenu> decoratedMenu)
	:
	MenuListDecorator(std::move (decoratedMenu)),
	pPlayer(Player::GetInstance(Maths::IRect(384, 267, 32, 44), "json/player.json"))
{
	std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>, Maths::IVec2D::Hash> buttonCharacteristics;
	int i = 0;
	for (auto pkmn : pPlayer->GetPokemonList()) {
		buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(150, 10 + 15 * i), std::make_pair<std::string, std::function<void(int&)>>(pkmn.GetName(), [&](int& out) {out = i; printf("%d", i); })));
		i++;
	}
	InitializeButtons(buttonCharacteristics);
}
