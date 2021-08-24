#include "FightingScene.h"
#include <random>
#include <iostream>

FightingScene::FightingScene(std::shared_ptr<Player> pPlayer)
	:
	Scene(pPlayer, Scene::SceneType::FightingScene),
	actionMenu(std::make_unique<ActionMenu>(std::make_unique<BasicMenu>())),
	itemTypeMenu(std::make_unique<ItemTypeMenu>(std::make_unique<BasicMenu>())),
	consumableMenu(std::make_unique<ItemMenu<Consumable>>(std::make_unique<BasicMenu>(), pPlayer)),
	ballMenu(std::make_unique<ItemMenu<Ball>>(std::make_unique<BasicMenu>(), pPlayer)),
	abilityMenu(std::make_unique<AbilityMenu>(std::make_unique<BasicMenu>(), pPlayer)),
	pokemonMenu(std::make_unique<PokemonMenu>(std::make_unique<BasicMenu>(), pPlayer)),
	enemyPkmnRect(500, 100, 128, 128),
	playerPkmnRect(200, 200, 128, 128),
	text(Maths::IRect(25, 500, 200, 75)),
	playerPkmnStatus(Maths::IRect(225, 350, 120, 50)),
	enemyPkmnStatus(Maths::IRect(525, 50, 120, 50))
{
	InitializeAbilityList();
	enemyPokemon = CreateRandomPokemon();
}

FightingScene::~FightingScene()
{
}

void FightingScene::Update()
{
	//Clear inputs
	ClearKbd();

	// FightingScene Routine
	abilityMenu = nullptr;
	pokemonMenu = nullptr;
	consumableMenu = nullptr;
	ballMenu = nullptr;
	abilityMenu = std::make_unique<AbilityMenu>(std::make_unique<BasicMenu>(), pPlayer);
	pokemonMenu = std::make_unique<PokemonMenu>(std::make_unique<BasicMenu>(), pPlayer);
	consumableMenu = std::make_unique<ItemMenu<Consumable>>(std::make_unique<BasicMenu>(), pPlayer);
	ballMenu = std::make_unique<ItemMenu<Ball>>(std::make_unique<BasicMenu>(), pPlayer);

	if (bWillChangeScene) {
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
		actionMenu->Update(output);
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
		abilityMenu->Update(output);
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
		ballMenu->Update(output);
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
		consumableMenu->Update(output);
		if (output != -1) {
			if (output == 30) {
				state = FightingState::ChoosingItemType;
			}
			else {
				attackTimer.ResetTimer(3.0f);
				pPlayer->UseItem(output);
				state = FightingState::PlayerHealing;
			}
		}
		break;
	case FightingState::ChoosingItemType:
		itemTypeMenu->Update(output);
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
		pokemonMenu->Update(output);
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
		text.Draw("You attacked the enemy Pokemon!", BLACK, GRAY, WHITE);
		break;
	case FightingState::PlayerHealing:
		text.Draw("You healed your pokemon!", BLACK, GRAY, WHITE);
		break;
	case FightingState::PlayerSwitching:
		text.Draw("You changed your lead pokemon!", BLACK, GRAY, WHITE);
		break;
	case FightingState::PlayerCatching:
		text.Draw("You threw a pokeball!", BLACK, GRAY, WHITE);
		break;
	case FightingState::PlayerFleeing:
		text.Draw("You are fleeing!", BLACK, GRAY, WHITE);
		break;
	case FightingState::EnemyAttacking:
		text.Draw("The enemy pokemon attacked you!", BLACK, GRAY, WHITE);
		break;
	case FightingState::EnemyCatched:
		text.Draw("You catched the enemy Pokemon!", BLACK, GRAY, WHITE);
		break;
	default:
		break;
	}
	
	playerPkmnStatus.Draw(std::to_string(pPlayer->GetPokemon().GetHP().x) + " / " + std::to_string(pPlayer->GetPokemon().GetHP().y) + " HP", BLACK, GRAY, WHITE);
	enemyPkmnStatus.Draw(std::to_string(enemyPokemon->GetHP().x) + " / " + std::to_string(enemyPokemon->GetHP().y) + " HP", BLACK, GRAY, WHITE);
}

std::shared_ptr<Pokemon> FightingScene::CreateRandomPokemon()
{
	std::shared_ptr<Pokemon> pkmn;
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> distPkmn(0, 2);
	std::uniform_int_distribution<int> distAbilities(0, 100);
	std::uniform_int_distribution<int> distType(0, 4);
	switch (distPkmn(rng)) {
	case 0:
		pkmn = nullptr;
		pkmn =std::make_shared<Pokemon>("Images/bulbasaur.png", "Bulbasaur", 1, Pokemon::Type::Grass);
		break;
	case 1:
		pkmn = nullptr;
		pkmn = std::make_shared<Pokemon>("Images/charmander.png", "Charmander", 2, Pokemon::Type::Fire);
		break;
	case 2:
		pkmn = nullptr;
		pkmn = std::make_shared<Pokemon>("Images/squirttle.png", "Squirttle", 3, Pokemon::Type::Water);
		break;
	default:
		break;
	}

	//New version
	std::map<int, Pokemon::Ability> pkmnPotentialAttacks;
	for (auto& entry : abilityList) {
		if (entry.second.GetType() == pkmn->GetType() || entry.second.GetType() == Pokemon::Type::Normal) {
			pkmnPotentialAttacks.insert(entry);
		}
	}
	pkmn->InitAbilities(pkmnPotentialAttacks);

	return pkmn;
}

void FightingScene::InitializeAbilityList()
{
	JSONParser::Reader jsonReader;
	jsonReader.ReadFile("json/abilityList.json");

	//Read Normal abilities
	auto& normalAbilities = jsonReader.GetValueOf("Normal");
	for (auto itr = normalAbilities.MemberBegin(); itr != normalAbilities.MemberEnd(); ++itr) {
		abilityList.insert(std::pair<int, Pokemon::Ability>(itr->value.GetArray()[2].GetInt(), Pokemon::Ability(itr->name.GetString(), itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt(), Pokemon::Type::Normal)));
	}

	//Read Fire Abilities
	auto& fireAbilities = jsonReader.GetValueOf("Fire");
	for (auto itr = fireAbilities.MemberBegin(); itr != fireAbilities.MemberEnd(); ++itr) {
		abilityList.insert(std::pair<int, Pokemon::Ability>(itr->value.GetArray()[2].GetInt(), Pokemon::Ability(itr->name.GetString(), itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt(), Pokemon::Type::Fire)));
	}

	//Read Water Abilities
	auto& waterAbilities = jsonReader.GetValueOf("Water");
	for (auto itr = waterAbilities.MemberBegin(); itr != waterAbilities.MemberEnd(); ++itr) {
		abilityList.insert(std::pair<int, Pokemon::Ability>(itr->value.GetArray()[2].GetInt(), Pokemon::Ability(itr->name.GetString(), itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt(), Pokemon::Type::Water)));
	}

	//Read Grass Abilities
	auto& grassAbilities = jsonReader.GetValueOf("Grass");
	for (auto itr = grassAbilities.MemberBegin(); itr != grassAbilities.MemberEnd(); ++itr) {
		abilityList.insert(std::pair<int, Pokemon::Ability>(itr->value.GetArray()[2].GetInt(), Pokemon::Ability(itr->name.GetString(), itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt(), Pokemon::Type::Grass)));
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
		std::cout << "Enemy has " << enemyPokemon->GetHP().x << " HP left!\n";
		std::cout << "Your pokemon has " << pPlayer->GetPokemon().GetHP().x << " HP Left!\n";
	}
}

void FightingScene::Heal()
{
	attackTimer.Update();
	if (attackTimer.IsTimerDown()) {
		enemyPokemon->Attack(pPlayer->GetPokemon());
		state = FightingState::EnemyAttacking;
		attackTimer.ResetTimer(3.0f);
		std::cout << "Enemy has " << enemyPokemon->GetHP().x << " HP left!\n";
		std::cout << "Your pokemon has " << pPlayer->GetPokemon().GetHP().x << " HP Left!\n";
	}
}

void FightingScene::Switch()
{
	attackTimer.Update();
	if (attackTimer.IsTimerDown()) {
		enemyPokemon->Attack(pPlayer->GetPokemon());
		state = FightingState::EnemyAttacking;
		attackTimer.ResetTimer(3.0f);
		std::cout << "Enemy has " << enemyPokemon->GetHP().x << " HP left!\n";
		std::cout << "Your pokemon has " << pPlayer->GetPokemon().GetHP().x << " HP Left!\n";
	}
}

void FightingScene::Capture()
{
	attackTimer.Update();
	if (attackTimer.IsTimerDown()) {
		if (pPlayer->CapturePokemon(chosenBall, *enemyPokemon)) {
			state = FightingState::EnemyCatched;
			fleeTimer.ResetTimer(2.0f);
		}
		else {
			enemyPokemon->Attack(pPlayer->GetPokemon());
			state = FightingState::EnemyAttacking;
			attackTimer.ResetTimer(3.0f);
			std::cout << "Enemy has " << enemyPokemon->GetHP().x << " HP left!\n";
			std::cout << "Your pokemon has " << pPlayer->GetPokemon().GetHP().x << " HP Left!\n";
		}
	}
}

void FightingScene::Flee()
{
	bWillChangeScene = true;
	newScene = Scene::SceneType::ExplorationScene;
	state = FightingState::ChoosingAction;
}
