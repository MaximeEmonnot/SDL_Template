#include "MenuListDecorator.h"

MenuListDecorator::MenuListDecorator(std::unique_ptr<AMenu> decoratedMenu)
	:
	pDecoratedMenu(std::move(decoratedMenu))
{
}

MenuListDecorator::~MenuListDecorator()
{
	printf("Destructor Menu\n");
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

void MenuListDecorator::InitializeButtons(const std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>, Maths::IVec2D::Hash>& buttonCharacteristics)
{
	for (auto& entry : buttonCharacteristics) {
		buttonList.emplace_back(entry.first, entry.second.first, entry.second.second);
	}
}
