#include "FightingScene.h"
#include <random>
#include <iostream>

FightingScene::FightingScene()
	:
	Scene(Scene::SceneType::FightingScene),
	pPlayer(Player::GetInstance(Maths::IRect(384, 267, 32, 44), "json/player.json")),
	pMouse(CoreSystem::Mouse::GetInstance()),
	actionMenu(std::make_unique<RightMenu>(std::make_unique<BasicMenu>())),
	itemTypeMenu(std::make_unique<ItemTypeMenu>(std::make_unique<BasicMenu>())),
	consumableMenu(std::make_unique<ItemMenu<Consumable>>(std::make_unique<BasicMenu>())),
	ballMenu(std::make_unique<ItemMenu<Ball>>(std::make_unique<BasicMenu>())),
	pokemonMenu(std::make_unique<PokemonMenu>(std::make_unique<BasicMenu>())),
	enemyPokemon(CreateRandomPokemon()),
	enemyPkmnRect(500, 50, 128, 128),
	playerPkmnRect(200, 200, 128, 128),
	attackPlayer("You attacked the enemy pokemon!", Maths::IRect(25, 500, 200, 75)),
	healPlayer("You healed your pokemon!", Maths::IRect(25, 500, 200, 75)),
	capturePlayer("You threw a pokeball!", Maths::IRect(25, 500, 200, 75)),
	attackEnemy("The enemy pokemon attacked you!", Maths::IRect(25, 500, 200, 75)),
	fleePlayer("You are fleeing!", Maths::IRect(25, 500, 200, 75)),
	successfulCatch("You catched the enemy Pokemon!", Maths::IRect(25, 500, 200, 75))
{
}

FightingScene::~FightingScene()
{
	delete enemyPokemon;
	pPlayer->Kill();
}

void FightingScene::Update()
{
	pokemonMenu = nullptr;
	consumableMenu = nullptr;
	ballMenu = nullptr;
	pokemonMenu = std::make_unique<PokemonMenu>(std::make_unique<BasicMenu>());
	consumableMenu = std::make_unique<ItemMenu<Consumable>>(std::make_unique<BasicMenu>());
	ballMenu = std::make_unique<ItemMenu<Ball>>(std::make_unique<BasicMenu>());

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
	else if (bIsHealing) {
		Heal();
	}
	else if (bIsCatching) {
		Capture();
	}
	else if (bEnemyIsAttacking) {
		attackTimer.Update();
		PlayAnimation(playerPkmnRect);
		if (attackTimer.IsTimerDown()) {
			playerPkmnRect.rect.x = 200;
			bEnemyIsAttacking = false;
		}
	}
	else if (bEnemyIsCatched) {
		fleeTimer.Update();
		if (fleeTimer.IsTimerDown()) {
			bWillChangeScene = true;
			newScene = Scene::SceneType::ExplorationScene;
			bEnemyIsCatched = false;
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
				if (output == 30) {
					bIsChoosingAbility = false;
				}
				else {
					bIsFighting = true;
					bIsChoosingAbility = false;
				}
			}
		}
		else if (bIsChoosingItemType) {
			itemTypeMenu->Update(output, pMouse);
			switch (output) {
			case 0: 
				bIsChoosingItem = true;
				bIsChoosingItemType = false;
				break;
			case 1:
				bIsChoosingBall = true;
				bIsChoosingItemType = false;
				break;
			case 30:
				bIsChoosingItemType = false;
				break;
			default :
				break;
			}
		}
		else if (bIsChoosingItem) {
			consumableMenu->Update(output, pMouse);
			if (output != -1) {
				if (output == 30) {
					bIsChoosingItem = false;
					bIsChoosingItemType = true;
				}
				else {
					chosenItem = output;
					bIsHealing = true;
					bIsChoosingItem = false;
				}
			}
		}
		else if (bIsChoosingBall) {
			ballMenu->Update(output, pMouse);
			if (output != -1) {
				if (output == 30) {
					bIsChoosingBall = false;
					bIsChoosingItemType = true;
				}
				else {
					chosenBall = output;
					bIsChoosingBall = false;
					bIsCatching = true;
				}
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
				bIsChoosingItemType = true;
				break;
			case 2:
				fleeTimer.ResetTimer(2.0f);
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
	pPlayer->GetPokemon().DrawFrontSprite(playerPkmnRect);
	if (!bIsCatching && !bEnemyIsCatched) enemyPokemon->DrawFrontSprite(enemyPkmnRect);

	if (!bIsFighting && !bIsHealing && !bEnemyIsAttacking && !bIsCatching && !bEnemyIsCatched) {
		if (bIsChoosingAbility) pokemonMenu->Draw(font);
		else if (bIsChoosingItemType) itemTypeMenu->Draw(font);
		else if (bIsChoosingItem) consumableMenu->Draw(font);
		else if (bIsChoosingBall) ballMenu->Draw(font);
		else actionMenu->Draw(font);
	}

	if (bPlayerIsAttacking) {
		attackPlayer.Draw(font, WHITE, WHITE);
	}
	if (bPlayerIsHealing) {
		healPlayer.Draw(font, WHITE, WHITE);
	}
	if (bPlayerIsCatching) {
		capturePlayer.Draw(font, WHITE, WHITE);
	}
	if (bEnemyIsCatched) {
		successfulCatch.Draw(font, WHITE, WHITE);
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
		return new Pokemon("Images/bulbasaur.png", "Bulbasaur", 1);
		break;
	case 1:
		return new Pokemon("Images/charmander.png", "Charmander", 2);
		break;
	case 2:
		return new Pokemon("Images/squirttle.png", "Squirttle", 3);
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

void FightingScene::Heal()
{
	if (!bPlayerIsHealing) {
		attackTimer.ResetTimer(3.0f);
		pPlayer->TEST_UseItem(chosenItem);
		bPlayerIsHealing = true;
	}
	else {
		attackTimer.Update();
		if (attackTimer.IsTimerDown()) {
			bPlayerIsHealing = false;
			enemyPokemon->Attack(pPlayer->GetPokemon());
			bEnemyIsAttacking = true;
			attackTimer.ResetTimer(3.0f);
			bIsHealing = false;
			std::cout << "Enemy has " << enemyPokemon->GetHP() << " HP left!\n";
			std::cout << "Your pokemon has " << pPlayer->GetPokemon().GetHP() << " HP Left!\n";
		}
	}
}

void FightingScene::Capture()
{
	if (!bPlayerIsCatching) {
		attackTimer.ResetTimer(3.0f);
		bPlayerIsCatching = true;
	}
	else {
		attackTimer.Update();
		if (attackTimer.IsTimerDown()) {
			bPlayerIsCatching = false;
			if (pPlayer->TEST_CapturePokemon(chosenBall, *enemyPokemon)) {
				bEnemyIsCatched = true;
				bIsCatching = false;
				fleeTimer.ResetTimer(2.0f);
			}
			else {
				enemyPokemon->Attack(pPlayer->GetPokemon());
				bEnemyIsAttacking = true;
				attackTimer.ResetTimer(3.0f);
				bIsCatching = false;
				std::cout << "Enemy has " << enemyPokemon->GetHP() << " HP left!\n";
				std::cout << "Your pokemon has " << pPlayer->GetPokemon().GetHP() << " HP Left!\n";
			}
		}
	}
}

void FightingScene::Flee()
{
	bWillChangeScene = true;
	newScene = Scene::SceneType::ExplorationScene;
	bIsFleeing = false;
}
