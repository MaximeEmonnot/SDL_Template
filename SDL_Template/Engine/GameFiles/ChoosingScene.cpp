#include "ChoosingScene.h"

ChoosingScene::ChoosingScene()
	:
	Scene(Scene::SceneType::ChoosingScene),
	choiceMenu(std::make_unique<ChoiceMenu>(std::make_unique<BasicMenu>())),
	choice0("Images/bulbasaur.png", "Bulbasaur", 1, Pokemon::Type::Grass),
	choice1("Images/charmander.png", "Charmander", 2, Pokemon::Type::Fire),
	choice2("Images/squirttle.png", "Squirttle", 3, Pokemon::Type::Water)
{
	choice0.LoadAbility(Pokemon::Ability("Charge", 55, 20, Pokemon::Type::Normal));
	choice0.LoadAbility(Pokemon::Ability("Vine Whip", 45, 25, Pokemon::Type::Grass));

	choice1.LoadAbility(Pokemon::Ability("Charge", 55, 20, Pokemon::Type::Normal));
	choice1.LoadAbility(Pokemon::Ability("Flame Wheel", 60, 25, Pokemon::Type::Fire));
	
	choice2.LoadAbility(Pokemon::Ability("Charge", 55, 20, Pokemon::Type::Normal));
	choice2.LoadAbility(Pokemon::Ability("Bubble", 40, 30, Pokemon::Type::Water));
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
