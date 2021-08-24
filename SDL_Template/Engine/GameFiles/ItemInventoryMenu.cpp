#include "ItemInventoryMenu.h"

ItemInventoryMenu::ItemInventoryMenu(std::unique_ptr<AMenu> decoratedMenu, std::shared_ptr<Player> pPlayer)
	:
	MenuListDecorator(std::move(decoratedMenu))
{
	std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>, Maths::IVec2D::Hash> buttonCharacteristics;
	int i = 0;
	for (auto& entry : pPlayer->GetItemList()) {
		buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(150, 10 + 15 * i), std::make_pair<std::string, std::function<void(int&)>>(entry.first->GetName() + "   x" + std::to_string(entry.second), [id = entry.first->GetID()](int& out) {out = id; printf("%d", id); })));
		i++;
	}
	InitializeButtons(buttonCharacteristics);
}
