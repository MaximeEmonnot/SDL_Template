#include "TitleScene.h"

TitleScene::TitleScene(const std::string& backgroundSprite)
	:
	Scene(backgroundSprite, Scene::SceneType::TitleScene),
	pWnd(CoreSystem::Window::GetInstance()),
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

void TitleScene::Update()
{
	bWillChangeScene = false;
	newScene = Scene::SceneType::TitleScene;
	
	int output = -1;
	menu->Update(output, pMouse);

	switch (output)
	{
	case 0:
		pWnd->ExitGame();
		break;
	case 1:
		bWillChangeScene = true;
		newScene = Scene::SceneType::ChoosingScene;
		break;
	case 2:
		break;
	default:
		break;
	}
}

void TitleScene::Draw()
{
	menu->Draw(font);
	title.Draw(font, WHITE, GRAY);
}