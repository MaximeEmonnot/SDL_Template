#include "BasicMenu.h"

BasicMenu::BasicMenu()
{
}

void BasicMenu::Destroy()
{
	printf("DESTRUCTION\n");
}

void BasicMenu::Update(std::shared_ptr<CoreSystem::Mouse> mouse, CoreSystem::Mouse::EventType e)
{
}

void BasicMenu::Draw(GraphicsEngine::Font& font)
{
}
