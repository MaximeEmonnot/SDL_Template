#include "ChoosingScene.h"

ChoosingScene::ChoosingScene(const std::string& backgroundSprite)
	:
	Scene(backgroundSprite, Scene::SceneType::ChoosingScene),
	pPlayer(Player::GetInstance(Maths::IRect(10, 10, 32, 32), "json/kirby.json")),
	pMouse(CoreSystem::Mouse::GetInstance()),
	choiceMenu(new BottomMenu(new BasicMenu())),
	choice0("Images/bulbasaur.png"),
	choice1("Images/charmander.png"),
	choice2("Images/squirttle.png")
{
}

ChoosingScene::~ChoosingScene()
{
	choiceMenu->Destroy();
	delete choiceMenu;
	pPlayer->Kill();
}

void ChoosingScene::Update()
{
	int output = -1;
	choiceMenu->Update(output, pMouse);

	if (output != -1) {
		pPlayer->SetPokemon(GetChoice(output));
		bWillChangeScene = true;
		newScene = Scene::SceneType::ExplorationScene;
	}
}

void ChoosingScene::Draw()
{
	choice0.DrawFrontSprite(Maths::IRect(150, 250, 128, 128));
	choice1.DrawFrontSprite(Maths::IRect(300, 250, 128, 128));
	choice2.DrawFrontSprite(Maths::IRect(450, 250, 128, 128));
	choiceMenu->Draw(font);
}

Pokemon& ChoosingScene::GetChoice(int choice)
{
	switch (choice) {
	case 1:
		return choice0;
		break;
	case 2:
		return choice1;
		break;
	case 3:
		return choice2;
		break;
	default:
		break;
	}
}
