#include "TitleScene.h"

TitleScene::TitleScene(const std::string& backgroundSprite)
	:
	Scene(backgroundSprite, Scene::SceneType::TitleScene),
	pWnd(CoreSystem::Window::GetInstance()),
	pMouse(CoreSystem::Mouse::GetInstance()),
	menu(new MiddleMenu(new BasicMenu())),
	title("Game title text", Maths::IRect(150, 25, 200, 100)),
	pPlayer(Player::GetInstance(Maths::IRect(384, 284, 32, 32), "json/kirby.json"))
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
	{
		bWillChangeScene = true;
		std::ifstream in("json/saveFile.json");
		if (in) {
			newScene = Scene::SceneType::ExplorationScene;
			pPlayer->InitFromJSON();
			printf("Loaded from save file!\n");
		}
		else {
			newScene = Scene::SceneType::ChoosingScene;
			printf("No save file found!\n");
		}
	}
		break;
	case 2:
		std::remove("json/saveFile.json");
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