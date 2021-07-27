#include "FightingScene.h"
#include <random>
#include <iostream>

FightingScene::FightingScene()
	:
	Scene(Scene::SceneType::FightingScene),
	pPlayer(Player::GetInstance(Maths::IRect(384, 267, 32, 44), "json/player.json")),
	pMouse(CoreSystem::Mouse::GetInstance()),
	actionMenu(std::make_unique<ActionMenu>(std::make_unique<BasicMenu>())),
	itemTypeMenu(std::make_unique<ItemTypeMenu>(std::make_unique<BasicMenu>())),
	consumableMenu(std::make_unique<ItemMenu<Consumable>>(std::make_unique<BasicMenu>())),
	ballMenu(std::make_unique<ItemMenu<Ball>>(std::make_unique<BasicMenu>())),
	abilityMenu(std::make_unique<AbilityMenu>(std::make_unique<BasicMenu>())),
	pokemonMenu(std::make_unique<PokemonMenu>(std::make_unique<BasicMenu>())),
	enemyPokemon(CreateRandomPokemon()),
	enemyPkmnRect(500, 50, 128, 128),
	playerPkmnRect(200, 200, 128, 128),
	text(Maths::IRect(25, 500, 200, 75))
{
}

FightingScene::~FightingScene()
{
	delete enemyPokemon;
	pPlayer->Kill();
}

void FightingScene::Update()
{
	abilityMenu = nullptr;
	pokemonMenu = nullptr;
	consumableMenu = nullptr;
	ballMenu = nullptr;
	abilityMenu = std::make_unique<AbilityMenu>(std::make_unique<BasicMenu>());
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

	int output = -1;

	switch (state) {
	case FightingState::PlayerFighting:
		Fight();
		break;
	case FightingState::PlayerHealing:
		Heal();
		break;
	case FightingState::PlayerCatching:
		Capture();
		break;
	case FightingState::PlayerSwitching:
		Switch();
		break;
	case FightingState::PlayerFleeing:
		fleeTimer.Update();
		if (fleeTimer.IsTimerDown()) {
			Flee();
		}
		break;
	case FightingState::EnemyAttacking:
		attackTimer.Update();
		PlayAnimation(playerPkmnRect);
		if (attackTimer.IsTimerDown()) {
			playerPkmnRect.rect.x = 200;
			state = FightingState::ChoosingAction;
		}
		break;
	case FightingState::EnemyCatched:
		fleeTimer.Update();
		if (fleeTimer.IsTimerDown()) {
			Flee();
		}
		break;
	case FightingState::ChoosingAction:
		actionMenu->Update(output, pMouse);
		switch (output)
		{
		case 0:
			state = FightingState::ChoosingAbility;
			break;
		case 1:
			state = FightingState::ChoosingItemType;
			break;
		case 2:
			state = FightingState::ChoosingPokemon;
			break;
		case 3:
			fleeTimer.ResetTimer(2.0f);
			state = FightingState::PlayerFleeing;
			break;
		default:
			break;
		}
		break;
	case FightingState::ChoosingAbility:
		abilityMenu->Update(output, pMouse);
		if (output != -1) {
			//pPlayer->GetPokemon().SelectedAttack(output) OR defined in PokemonMenu;
			if (output == 30) {
				state = FightingState::ChoosingAction;
			}
			else {
				attackTimer.ResetTimer(3.0f);
				pPlayer->GetPokemon().Attack(*enemyPokemon);
				state = FightingState::PlayerFighting;
			}
		}
		break;
	case FightingState::ChoosingBall:
		ballMenu->Update(output, pMouse);
		if (output != -1) {
			if (output == 30) {
				state = FightingState::ChoosingItemType;
			}
			else {
				chosenBall = output;
				attackTimer.ResetTimer(3.0f);
				state = FightingState::PlayerCatching;
			}
		}
		break;
	case FightingState::ChoosingConsumable:
		consumableMenu->Update(output, pMouse);
		if (output != -1) {
			if (output == 30) {
				state = FightingState::ChoosingItemType;
			}
			else {
				attackTimer.ResetTimer(3.0f);
				pPlayer->TEST_UseItem(output);
				state = FightingState::PlayerHealing;
			}
		}
		break;
	case FightingState::ChoosingItemType:
		itemTypeMenu->Update(output, pMouse);
		switch (output) {
		case 0:
			state = FightingState::ChoosingConsumable;
			break;
		case 1:
			state = FightingState::ChoosingBall;
			break;
		case 30:
			state = FightingState::ChoosingAction;
			break;
		default:
			break;
		}
		break;
	case FightingState::ChoosingPokemon:
		pokemonMenu->Update(output, pMouse);
		if (output != -1) {
			if (output == 30) {
				state = FightingState::ChoosingAction;
			}
			else {
				attackTimer.ResetTimer(3.0f);
				pPlayer->SetFirstPokemon(output);
				state = FightingState::PlayerSwitching;
			}
		}
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
	pPlayer->GetPokemon().DrawFrontSprite(playerPkmnRect);
	if (state != FightingState::PlayerCatching && state != FightingState::EnemyCatched) enemyPokemon->DrawFrontSprite(enemyPkmnRect);

	switch (state) {
	case FightingState::ChoosingAction:
		actionMenu->Draw();
		break;
	case FightingState::ChoosingAbility:
		abilityMenu->Draw();
		break;
	case FightingState::ChoosingItemType:
		itemTypeMenu->Draw();
		break;
	case FightingState::ChoosingConsumable:
		consumableMenu->Draw();
		break;
	case FightingState::ChoosingBall:
		ballMenu->Draw();
		break;
	case FightingState::ChoosingPokemon:
		pokemonMenu->Draw();
		break;
	case FightingState::PlayerFighting:
		text.Draw("You attacked the enemy Pokemon!", WHITE, WHITE);
		break;
	case FightingState::PlayerHealing:
		text.Draw("You healed your pokemon!", WHITE, WHITE);
		break;
	case FightingState::PlayerSwitching:
		text.Draw("You changed your lead pokemon!", WHITE, WHITE);
		break;
	case FightingState::PlayerCatching:
		text.Draw("You threw a pokeball!", WHITE, WHITE);
		break;
	case FightingState::PlayerFleeing:
		text.Draw("You are fleeing!", WHITE, WHITE);
		break;
	case FightingState::EnemyAttacking:
		text.Draw("The enemy pokemon attacked you!", WHITE, WHITE);
		break;
	case FightingState::EnemyCatched:
		text.Draw("You catched the enemy Pokemon!", WHITE, WHITE);
		break;
	default:
		break;
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
	attackTimer.Update();
	PlayAnimation(enemyPkmnRect);
	if (attackTimer.IsTimerDown()) {
		enemyPkmnRect.rect.x = 500;
		enemyPokemon->Attack(pPlayer->GetPokemon());
		state = FightingState::EnemyAttacking;
		attackTimer.ResetTimer(3.0f);
		std::cout << "Enemy has " << enemyPokemon->GetHP() << " HP left!\n";
		std::cout << "Your pokemon has " << pPlayer->GetPokemon().GetHP() << " HP Left!\n";
	}
}

void FightingScene::Heal()
{
	attackTimer.Update();
	if (attackTimer.IsTimerDown()) {
		enemyPokemon->Attack(pPlayer->GetPokemon());
		state = FightingState::EnemyAttacking;
		attackTimer.ResetTimer(3.0f);
		std::cout << "Enemy has " << enemyPokemon->GetHP() << " HP left!\n";
		std::cout << "Your pokemon has " << pPlayer->GetPokemon().GetHP() << " HP Left!\n";
	}
}

void FightingScene::Switch()
{
	attackTimer.Update();
	if (attackTimer.IsTimerDown()) {
		enemyPokemon->Attack(pPlayer->GetPokemon());
		state = FightingState::EnemyAttacking;
		attackTimer.ResetTimer(3.0f);
		std::cout << "Enemy has " << enemyPokemon->GetHP() << " HP left!\n";
		std::cout << "Your pokemon has " << pPlayer->GetPokemon().GetHP() << " HP Left!\n";
	}
}

void FightingScene::Capture()
{
	attackTimer.Update();
	if (attackTimer.IsTimerDown()) {
		if (pPlayer->TEST_CapturePokemon(chosenBall, *enemyPokemon)) {
			state = FightingState::EnemyCatched;
			fleeTimer.ResetTimer(2.0f);
		}
		else {
			enemyPokemon->Attack(pPlayer->GetPokemon());
			state = FightingState::EnemyAttacking;
			attackTimer.ResetTimer(3.0f);
			std::cout << "Enemy has " << enemyPokemon->GetHP() << " HP left!\n";
			std::cout << "Your pokemon has " << pPlayer->GetPokemon().GetHP() << " HP Left!\n";
		}
	}
}

void FightingScene::Flee()
{
	bWillChangeScene = true;
	newScene = Scene::SceneType::ExplorationScene;
	state = FightingState::ChoosingAction;
}
