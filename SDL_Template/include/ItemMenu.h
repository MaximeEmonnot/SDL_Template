#pragma once
#include "MenuListDecorator.h"
#include "Player.h"

template<class T>
class ItemMenu : public MenuListDecorator {
public:
	ItemMenu(std::unique_ptr<AMenu> decoratedMenu)
		:
		MenuListDecorator(std::move(decoratedMenu)),
		pPlayer(Player::GetInstance(Maths::IRect(384, 267, 32, 44), "json/player.json"))
	{
		std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>, Maths::IVec2D::Hash> buttonCharacteristics;
		int i = 0;
		for (auto& entry : pPlayer->GetItemList()) {
			std::shared_ptr<T> item = std::dynamic_pointer_cast<T, Item>(entry.first);
			if (item != nullptr) {
				buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(625, 400 + 15 * i), std::make_pair<std::string, std::function<void(int&)>>(item->GetName() + "   x" + std::to_string(entry.second), [id = item->GetID()](int& out) {out = id; printf("%d", id); })));
				i++;
			}
		}
		buttonCharacteristics.insert(std::pair<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>>(Maths::IVec2D(700, 575), std::make_pair<std::string, std::function<void(int&)>>("Back", [&](int& out) {printf("Back!\n"); out = 30; })));
		InitializeButtons(buttonCharacteristics);
	}
private:
	std::shared_ptr<Player> pPlayer;
};