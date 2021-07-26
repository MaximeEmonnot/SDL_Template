#include "TitleScene.h"

TitleScene::TitleScene()
	:
	Scene(Scene::SceneType::TitleScene),
	pWnd(CoreSystem::Window::GetInstance()),
	pMouse(CoreSystem::Mouse::GetInstance()),
	menu(std::make_unique<MiddleMenu>(std::make_unique<BasicMenu>())),
	title("Game title text", Maths::IRect(150, 25, 200, 100)),
	pPlayer(Player::GetInstance(Maths::IRect(384, 267, 32, 44), "json/player.json"))
{
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
		std::ifstream inSF("json/saveFile.json");
		std::ifstream inMC("json/mapCoords.json");
		if (inSF && inMC) {
			newScene = Scene::SceneType::ExplorationScene;
			pPlayer->TEST_bInitFromJSON = true;
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
		std::remove("json/mapCoords.json");
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