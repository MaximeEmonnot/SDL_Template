#include "ItemInventory.h"

ItemInventory::ItemInventory(std::unique_ptr<AMenu> decoratedMenu)
	:
	MenuListDecorator(std::move(decoratedMenu)),
	pPlayer(Player::GetInstance(Maths::IRect(384, 284, 32, 32), "json/kirby.json"))
{
	std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>, Maths::IVec2D::Hash> buttonCharacteristics;
	for (size_t i = 0; i < pPlayer->GetItemList().size(); i++) {
		buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(150, 10 + 15*i), std::make_pair<std::string, std::function<void(int&)>>("item" + std::to_string(i), [&](int& out) {out = i; })));
	}
	InitializeButtons(buttonCharacteristics);
}
