#include "FightingScene.h"
#include <random>
#include <iostream>

FightingScene::FightingScene()
	:
	Scene(Scene::SceneType::FightingScene),
	pPlayer(Player::GetInstance(Maths::IRect(384, 284, 32, 32), "json/kirby.json")),
	pMouse(CoreSystem::Mouse::GetInstance()),
	actionMenu(std::make_unique<RightMenu>(std::make_unique<BasicMenu>())),
	enemyPokemon(CreateRandomPokemon()),
	enemyPkmnRect(500, 50, 128, 128),
	playerPkmnRect(200, 200, 128, 128),
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
		attackTimer.Update();
		PlayAnimation(playerPkmnRect);
		if (attackTimer.IsTimerDown()) {
			playerPkmnRect.rect.x = 200;
			bEnemyIsAttacking = false;
		}
	}
	else if (bIsFleeing) {
		fleeTimer.Update();
		if (fleeTimer.IsTimerDown()) {
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
				fleeTimer.ResetTimer(2.0f);
				bIsFleeing = true;
				break;
			case 2:
				bIsChoosingBall = true;
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
	pPlayer->GetPokemon().DrawFrontSprite(playerPkmnRect);
	enemyPokemon->DrawFrontSprite(enemyPkmnRect);

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

void FightingScene::PlayAnimation(Maths::IRect& rect)
{
	rect.rect.x += currentDir;
	
	animationTimer.Update();
	if (animationTimer.IsTimerDown()) {
		animationTimer.ResetTimer(0.10f);
		currentDir *= -1;
	}
}

void FightingScene::Fight()
{
	if (!bPlayerIsAttacking) {
		attackTimer.ResetTimer(3.0f);
		pPlayer->GetPokemon().Attack(*enemyPokemon);
		bPlayerIsAttacking = true;
	}
	else {
		attackTimer.Update();
		PlayAnimation(enemyPkmnRect);
		if (attackTimer.IsTimerDown()) {
			enemyPkmnRect.rect.x = 500;
			bPlayerIsAttacking = false;
			enemyPokemon->Attack(pPlayer->GetPokemon());
			bEnemyIsAttacking = true;
			attackTimer.ResetTimer(3.0f);
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
