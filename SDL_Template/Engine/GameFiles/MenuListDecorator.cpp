#include "MenuListDecorator.h"

MenuListDecorator::MenuListDecorator(std::unique_ptr<AMenu> decoratedMenu)
	:
	pDecoratedMenu(std::move(decoratedMenu))
{
}

MenuListDecorator::~MenuListDecorator()
{
	buttonList.clear();
}

void MenuListDecorator::Update(int& output, std::shared_ptr<CoreSystem::Mouse> mouse, CoreSystem::Mouse::EventType e)
{
	if (e == CoreSystem::Mouse::EventType::None) e = mouse->Read();
	if (e == CoreSystem::Mouse::EventType::LPress) {
		pDecoratedMenu->Update(output, mouse, e);
		for (auto& button : buttonList) {
			if (button.OnClick(mouse->GetMousePos())) {
				button.ComputeFunction(output);
				break;
			}
		}
	}
}

void MenuListDecorator::Draw()
{
	pDecoratedMenu->Draw();
	for (auto& button : buttonList) {
		if (button.GetState()) {
			button.Draw(GREEN);
		}
		else {
			button.Draw(GRAY);
		}
	}
}

void MenuListDecorator::InitializeButtons(const std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>, Maths::IVec2D::Hash>& buttonCharacteristics)
{
	for (auto& entry : buttonCharacteristics) {
		buttonList.emplace_back(entry.first, entry.second.first, entry.second.second);
	}
}