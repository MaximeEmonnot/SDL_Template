#pragma once
#include "AMenu.h"
#include <unordered_map>

class MenuListDecorator : public AMenu
{
public:
	MenuListDecorator(std::unique_ptr<AMenu> decoratedMenu);
	~MenuListDecorator() override;

	void Update(int& output, std::shared_ptr<CoreSystem::Mouse> mouse, CoreSystem::Mouse::EventType e = CoreSystem::Mouse::EventType::None) override;
	void Draw(GraphicsEngine::Font& font) override;

protected:
	void InitializeButtons(const std::unordered_map<Maths::IVec2D, std::pair<std::string, std::function<void(int&)>>, Maths::IVec2D::Hash>& buttonCharacteristics);

protected:
	std::unique_ptr<AMenu> pDecoratedMenu;
	std::vector<Button> buttonList;
};