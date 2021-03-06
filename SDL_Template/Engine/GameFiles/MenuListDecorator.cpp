#include "MenuListDecorator.h"

MenuListDecorator::MenuListDecorator(AMenu* decoratedMenu)
	:
	pDecoratedMenu(decoratedMenu)
{
}

void MenuListDecorator::Destroy()
{	
	buttonList.clear();
	buttonList.~vector();
	pDecoratedMenu->Destroy();
	delete pDecoratedMenu;
}

void MenuListDecorator::Update(std::shared_ptr<CoreSystem::Mouse> mouse, CoreSystem::Mouse::EventType e)
{
	if (e == CoreSystem::Mouse::EventType::None) e = mouse->Read();
	if (e == CoreSystem::Mouse::EventType::LPress) {
		pDecoratedMenu->Update(mouse, e);
		for (auto& button : buttonList) {
			if (button.OnClick(mouse->GetMousePos())) {
				button.ComputeFunction();
				break;
			}
		}
	}
}

void MenuListDecorator::Draw(GraphicsEngine::Font& font)
{
	pDecoratedMenu->Draw(font);
	for (auto& button : buttonList) {
		if (button.GetState()) {
			button.Draw(GREEN, font);
		}
		else {
			button.Draw(GRAY, font);
		}
	}
}

void MenuListDecorator::InitializeButtons(const std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void()>>, Maths::IVec2D::Hash>& buttonCharacteristics)
{
	for (auto& entry : buttonCharacteristics) {
		buttonList.emplace_back(entry.first, entry.second.first, entry.second.second);
	}
}
