#include "TitleScene.h"

TitleScene::TitleScene(const std::string& backgroundSprite)
	:
	Scene(backgroundSprite), 
	pMouse(CoreSystem::Mouse::GetInstance()),
	menu(new MiddleMenu(new BasicMenu())),
	title("Game title text", Maths::IRect(150, 25, 200, 100))
{
}

TitleScene::~TitleScene()
{
	menu->Destroy();
	delete menu;
}

void TitleScene::Update(int& output)
{
	menu->Update(output, pMouse);
}

void TitleScene::Draw()
{
	menu->Draw(font);
	title.Draw(font, WHITE, GRAY);
}