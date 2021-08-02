#include "ExplorationScene.h"

ExplorationScene::ExplorationScene()
	:
	Scene(Scene::SceneType::ExplorationScene),
	explorationMenu(std::make_unique<ExplorationMenu>(std::make_unique<BasicMenu>())),
	itemInventoryMenu(std::make_unique<ItemInventoryMenu>(std::make_unique<BasicMenu>())),
	pokemonInventoryMenu(std::make_unique<PokemonInventoryMenu>(std::make_unique<BasicMenu>())),
	pGrid(Grid::GetInstance()),
	pPlayer(Player::GetInstance(Maths::IRect(384, 267, 32, 44), "json/player.json")),
	pTimer(CoreSystem::Timer::GetInstance()),
	pMouse(CoreSystem::Mouse::GetInstance()),
	pWnd(CoreSystem::Window::GetInstance()),
	pKbd(CoreSystem::Keyboard::GetInstance()),
	pSoundSystem(SoundEngine::SoundSystem::GetInstance())
{
}

ExplorationScene::~ExplorationScene()
{
	pGrid->Kill();
}

void ExplorationScene::Update()
{
	//Song
	if (!bIsPlayingSong) {
		if (pTimer->IsNightTime()) {
			pSoundSystem->PlaySound(pSoundSystem->ConstructNewSong("music/nightwalk.wav", MUSIC), -1);
		}
		else {
			pSoundSystem->PlaySound(pSoundSystem->ConstructNewSong("music/daywalk.wav", MUSIC), -1);
		}
		bIsPlayingSong = true;
	}
	else {
		if (bCurrentDayState != pTimer->IsNightTime()) {
			pSoundSystem->StopSounds();
			bCurrentDayState = pTimer->IsNightTime();
			bIsPlayingSong = false;
		}
	}

	//Update menus
	itemInventoryMenu = nullptr;
	pokemonInventoryMenu = nullptr;
	itemInventoryMenu = std::make_unique<ItemInventoryMenu>(std::make_unique<BasicMenu>());
	pokemonInventoryMenu = std::make_unique<PokemonInventoryMenu>(std::make_unique<BasicMenu>());

	//Init from json
	if (pPlayer->TEST_bInitFromJSON) {
		pPlayer->TEST_bInitFromJSON = false;
		InitFromJSON();
	}

	bWillChangeScene = false;

	pPlayer->Update(pTimer->DeltaTime());

	int output = -1;

	switch (state) {
	case MenuState::None:
		pPlayer->Move();
		pGrid->Update();
		//house.Update();
		break;
	case MenuState::ShowingMenu:
		explorationMenu->Update(output, pMouse);
		switch (output) {
		case 0:
			pWnd->ExitGame();
			break;
		case 1:
			state = MenuState::ShowingItemInventory;
			break;
		case 2:
			state = MenuState::ShowingPokemonInventory;
			break;
		case 3:
			pPlayer->SaveToJSON();
			SaveToJSON();
			break;
		case 4:
			bWillChangeScene = true;
			state = MenuState::None;
			bIsPlayingSong = false;
			pSoundSystem->StopSounds();
			newScene = Scene::SceneType::TitleScene;
			break;
		default:
			break;
		}
		break;
	case MenuState::ShowingItemInventory:
		itemInventoryMenu->Update(output, pMouse);
		if (output != -1) {
			if (pPlayer->TEST_CanUseItem(output)) {
				chosenItem = output;
				state = MenuState::HealingPokemon;
				printf("item clicked!\n");
			}
			else {
				printf("Cant use that item yet!\n");
			}
		}
		break;
	case MenuState::ShowingPokemonInventory:
		pokemonInventoryMenu->Update(output, pMouse);
		if (output != -1) {
			printf("Pokemon clicked!\n");
		}
		break;
	case MenuState::HealingPokemon:
		pokemonInventoryMenu->Update(output, pMouse);
		if (output != -1) {
			pPlayer->TEST_UseItem(chosenItem, output);
			state = MenuState::None;
			printf("Pokemon healed !");
		}
	default:
		break;
	}

	if (pGrid->PlayerTriggersFight() && !pPlayer->GetPokemon().IsDead()) {
		bWillChangeScene = true;
		bIsPlayingSong = false;
		pSoundSystem->StopSounds();
		newScene = Scene::SceneType::FightingScene;
	}

	auto e = pKbd->ReadKey();

	if (e.keycode == SDL_SCANCODE_ESCAPE && e.type == CoreSystem::Keyboard::Event::Type::Pressed) {
		switch (state)
		{
		case ExplorationScene::MenuState::ShowingMenu:
			state = MenuState::None;
			break;
		case ExplorationScene::MenuState::ShowingItemInventory:
		case ExplorationScene::MenuState::ShowingPokemonInventory:
		case ExplorationScene::MenuState::None:
			state = MenuState::ShowingMenu;
			break;
		case MenuState::HealingPokemon:
			state = MenuState::ShowingItemInventory;
			break;
		default:
			break;
		}

	}
}

void ExplorationScene::Draw()
{
	pGrid->Draw();
	//house.Draw();
	pPlayer->Draw();

	switch (state) {
	case MenuState::ShowingMenu:
		explorationMenu->Draw();
		break;
	case MenuState::ShowingItemInventory:
		itemInventoryMenu->Draw();
		break;
	case MenuState::ShowingPokemonInventory:
		pokemonInventoryMenu->Draw();
		break;
	case MenuState::HealingPokemon:
		itemInventoryMenu->Draw();
		pokemonInventoryMenu->Draw();
		break;
	default:
		break;
	}
	pFont->DrawText(Maths::IVec2D(10, 10), (std::string("X     ") + std::to_string(pGrid->xOffset) + "\n" + std::string("Y     ") + std::to_string(pGrid->yOffset)).c_str(), RED);
}

void ExplorationScene::SaveToJSON()
{
	pGrid->SaveToJSON();
	pPlayer->SaveToJSON();
}

void ExplorationScene::InitFromJSON()
{
	pGrid->InitFromJSON();
	pPlayer->InitFromJSON();	
}