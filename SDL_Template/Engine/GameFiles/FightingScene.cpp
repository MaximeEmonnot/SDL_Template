#include "FightingScene.h"
#include <random>
#include <iostream>

FightingScene::FightingScene(const std::string& backgroundSprite)
	:
	Scene(backgroundSprite, Scene::SceneType::FightingScene),
	pPlayer(Player::GetInstance(Maths::IRect(384, 284, 32, 32), "json/kirby.json")),
	pMouse(CoreSystem::Mouse::GetInstance()),
	actionMenu(std::make_unique<RightMenu>(std::make_unique<BasicMenu>())),
	enemyPokemon(CreateRandomPokemon()),
	attackPlayer("You attacked the enemy pokemon!", Maths::IRect(25, 500, 200, 75)),
	attackEnemy("The enemy pokemon attacked you!", Maths::IRect(25, 500, 200, 75)),
	fleePlayer("You are fleeing!", Maths::IRect(25,500,200,75)),
	pokemonMenu(std::make_unique<PokemonMenu>(std::make_unique<BasicMenu>(), pPlayer->GetPokemon()))
{
}

FightingScene::~FightingScene()
{
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
	if (bIsFighting) {
		Fight();
	}
	else if (bEnemyIsAttacking) {
		timer.Update(attackTimer);
		if (timer.IsTimerDown(attackTimer)) {
			bEnemyIsAttacking = false;
		}
	}
	else if (bIsFleeing) {
		timer.Update(fleeTimer);
		if (timer.IsTimerDown(fleeTimer)) {
			Flee();
		}
	}
	else {
		int output = -1;

		if (bIsChoosingAbility) {
			pokemonMenu->Update(output, pMouse);
			if (output != -1) {
				//pPlayer->GetPokemon().SelectedAttack(output) OR defined in PokemonMenu;
				bIsFighting = true;
				bIsChoosingAbility = false;
			}
		}
		else {
			actionMenu->Update(output, pMouse);
			switch (output)
			{
			case 0:
				bIsChoosingAbility = true;
				break;
			case 1:
				timer.ResetTimer(fleeTimer, 2.0f);
				bIsFleeing = true;
				break;
			default:
				break;
			}
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
}

void FightingScene::Draw()
{
	pPlayer->GetPokemon().DrawFrontSprite(Maths::IRect(200, 200, 128, 128));
	enemyPokemon->DrawFrontSprite(Maths::IRect(500, 50, 128, 128));

	if (!bIsFighting && !bEnemyIsAttacking) {
		if (bIsChoosingAbility) 
			pokemonMenu->Draw(font);
		else actionMenu->Draw(font);
	}

	if (bPlayerIsAttacking) {
		attackPlayer.Draw(font, WHITE, WHITE);
	}
	if (bEnemyIsAttacking) {
		attackEnemy.Draw(font, WHITE, WHITE);
	}
	if (bIsFleeing) {
		fleePlayer.Draw(font, WHITE, WHITE);
	}
}

Pokemon* FightingScene::CreateRandomPokemon()
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(0, 2);
	switch (dist(rng)) {
	case 0:
		return new Pokemon("Images/bulbasaur.png", 1);
		break;
	case 1:
		return new Pokemon("Images/charmander.png", 2);
		break;
	case 2:
		return new Pokemon("Images/squirttle.png", 3);
		break;
	default:
		return new Pokemon();
		break;
	}
}

void FightingScene::Fight()
{
	if (!bPlayerIsAttacking) {
		timer.ResetTimer(attackTimer, 3.0f);
		pPlayer->GetPokemon().Attack(*enemyPokemon);
		bPlayerIsAttacking = true;
	}
	else {
		timer.Update(attackTimer);
		if (timer.IsTimerDown(attackTimer)) {
			bPlayerIsAttacking = false;
			enemyPokemon->Attack(pPlayer->GetPokemon());
			bEnemyIsAttacking = true;
			timer.ResetTimer(attackTimer, 3.0f);
			bIsFighting = false;
			std::cout << "Enemy has " << enemyPokemon->GetHP() << " HP left!\n";
			std::cout << "Your pokemon has " << pPlayer->GetPokemon().GetHP() << " HP Left!\n";
		}
	}
}

void FightingScene::Flee()
{
	bWillChangeScene = true;
	newScene = Scene::SceneType::ExplorationScene;
	bIsFleeing = false;
}
