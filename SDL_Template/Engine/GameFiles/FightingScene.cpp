#include "FightingScene.h"
#include <random>
#include <iostream>

FightingScene::FightingScene(const std::string& backgroundSprite)
	:
	Scene(backgroundSprite, Scene::SceneType::FightingScene),
	pPlayer(Player::GetInstance(Maths::IRect(10, 10, 32, 32), "json/kirby.json")),
	pMouse(CoreSystem::Mouse::GetInstance()),
	actionMenu(new RightMenu(new BasicMenu())),
	enemyPokemon(CreateRandomPokemon())
{
}

FightingScene::~FightingScene()
{
	actionMenu->Destroy();
	delete actionMenu;
	delete enemyPokemon;
	pPlayer->Kill();
}

void FightingScene::Update()
{
	if (bWillChangeScene) {
		delete enemyPokemon;
		enemyPokemon = nullptr;
		enemyPokemon = CreateRandomPokemon();
		newScene = Scene::SceneType::ExplorationScene;
		bWillChangeScene = false;
	}
	int output = -1;
	actionMenu->Update(output, pMouse);
	switch (output)
	{
	case 0:
		Fight();
		break;
	case 1:
		Flee();
		break;
	default:
		break;
	}
	if (enemyPokemon->IsDead()) {
		std::cout << "You won!\n";
		Flee();
	}
	else if (pPlayer->GetPokemon().IsDead()) {
		std::cout << "You lost!\n";
		Flee();
	}
}

void FightingScene::Draw()
{
	pPlayer->GetPokemon().DrawFrontSprite(Maths::IRect(200, 200, 128, 128));
	enemyPokemon->DrawFrontSprite(Maths::IRect(500, 50, 128, 128));
	actionMenu->Draw(font);
}

Pokemon* FightingScene::CreateRandomPokemon()
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(0, 2);
	switch (dist(rng)) {
	case 0:
		return new Pokemon("Images/bulbasaur.png");
		break;
	case 1:
		return new Pokemon("Images/charmander.png");
		break;
	case 2:
		return new Pokemon("Images/squirttle.png");
		break;
	default:
		return new Pokemon();
		break;
	}
}

void FightingScene::Fight()
{
	pPlayer->GetPokemon().Attack(*enemyPokemon);
	enemyPokemon->Attack(pPlayer->GetPokemon());
	std::cout << "Enemy has " << enemyPokemon->GetHP() << " HP left!\n";
	std::cout << "Your pokemon has " << pPlayer->GetPokemon().GetHP() << " HP Left!\n";
}

void FightingScene::Flee()
{
	bWillChangeScene = true;
	newScene = Scene::SceneType::ExplorationScene;
}
