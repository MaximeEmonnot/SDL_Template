#include "ChoosingScene.h"

ChoosingScene::ChoosingScene()
	:
	Scene(Scene::SceneType::ChoosingScene),
	pPlayer(Player::GetInstance(Maths::IRect(384, 267, 32, 44), "json/player.json")),
	pMouse(CoreSystem::Mouse::GetInstance()),
	choiceMenu(std::make_unique<BottomMenu>(std::make_unique<BasicMenu>())),
	choice0("Images/bulbasaur.png", "Bulbasaur", 1),
	choice1("Images/charmander.png", "Charmander", 2),
	choice2("Images/squirttle.png", "Squirttle", 3)
{
}

ChoosingScene::~ChoosingScene()
{
	pPlayer->Kill();
}

void ChoosingScene::Update()
{
	int output = -1;
	choiceMenu->Update(output, pMouse);

	if (output != -1) {
		pPlayer->AddPokemon(GetChoice(output));
		bWillChangeScene = true;
		newScene = Scene::SceneType::ExplorationScene;
	}
}

void ChoosingScene::Draw()
{
	choice0.DrawFrontSprite(Maths::IRect(150, 250, 128, 128));
	choice1.DrawFrontSprite(Maths::IRect(300, 250, 128, 128));
	choice2.DrawFrontSprite(Maths::IRect(450, 250, 128, 128));
	choiceMenu->Draw();
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
