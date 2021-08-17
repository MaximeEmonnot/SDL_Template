#include "ExplorationScene.h"

ExplorationScene::ExplorationScene()
	:
	Scene(Scene::SceneType::ExplorationScene),
	explorationMenu(std::make_unique<ExplorationMenu>(std::make_unique<BasicMenu>())),
	itemInventoryMenu(std::make_unique<ItemInventoryMenu>(std::make_unique<BasicMenu>())),
	pokemonInventoryMenu(std::make_unique<PokemonInventoryMenu>(std::make_unique<BasicMenu>())),
	pWorld(World::GetInstance()),
	text(Maths::IRect(25, 500, 200, 75))
{
}

ExplorationScene::~ExplorationScene()
{
	pWorld->Kill();
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

	//Update Timers
	transitionTimer.Update();
	saveTimer.Update();

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
	{
		float speed = 1.0f;
		switch (pPlayer->GetLocomotionState()) {
		case Player::LocomotionState::Walking:
			speed = 1.0f;
			break;
		case Player::LocomotionState::OnWater:
		case Player::LocomotionState::Running:
			speed = 1.5f;
			break;
		case Player::LocomotionState::Biking:
			speed = 2.0f;
			break;
		default:
			break;
		}

		if (transitionTimer.IsTimerDown() && !pPlayer->IsTalking()) pPlayer->Move();

		if (bIsInsideHouse) {
			if (transitionTimer.IsTimerDown()) house.Update(speed);

			//New
			if (house.GoOutside()) {
				transitionTimer.ResetTimer(0.75f);
				bInTransition = true;
			}
			if (bInTransition)
			{
				if (transitionTimer.IsTimerDown()) {
					bInTransition = false;
					bIsInsideHouse = false;
					transitionTimer.ResetTimer(1.0f);
				}
			}
		}
		else {
			if (transitionTimer.IsTimerDown()) pWorld->Update(speed);

			if (pWorld->GoInside()) {
				transitionTimer.ResetTimer(0.75f);
				bInTransition = true;
			}
			if (bInTransition) {
				if (transitionTimer.IsTimerDown()) {
					bInTransition = false;
					bIsInsideHouse = true;
					transitionTimer.ResetTimer(1.0f);
				}
			}
		}
	}
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
			pPlayer->UpdateBike();
			state = MenuState::None;
			break;
		case 4:
			saveTimer.ResetTimer(2.5f);
			pPlayer->SaveToJSON();
			SaveToJSON();
			break;
		case 5:
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
			pPlayer->UsePokemon(output);
			state = MenuState::None;
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

	if (pWorld->PlayerTriggersFight() && !pPlayer->GetPokemon().IsDead()) {
		bWillChangeScene = true;
		bIsPlayingSong = false;
		pSoundSystem->StopSounds();
		newScene = Scene::SceneType::FightingScene;
	}

	auto e = pKbd->ReadKey();

	if (transitionTimer.IsTimerDown() && !pPlayer->IsTalking()) {
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
}

void ExplorationScene::Draw()
{
	pWorld->BlendSpriteTo(GraphicsEngine::Color(255, 255, 255, 255));
	pPlayer->BlendSpriteTo(GraphicsEngine::Color(255, 255, 255, 255));

	if(bIsInsideHouse) house.Draw();
	else {
		if (pTimer->IsNightTime())
		{
			pWorld->BlendSpriteTo(GraphicsEngine::Color(64, 64, 128, 128));
			pPlayer->BlendSpriteTo(GraphicsEngine::Color(64, 64, 128, 128));
		}
		pWorld->Draw();
		pPlayer->DrawReflection();
	}
	pPlayer->Draw();

	if (bInTransition) {
		pGfx->FadeOutScreen(transitionTimer.GetTimer() / 1.5f);
	}
	else if (!transitionTimer.IsTimerDown()) {
		pGfx->FadeInScreen(transitionTimer.GetTimer() / 1.5f);
	}

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
	pFont->DrawText(Maths::IVec2D(10, 10), (std::string("X     ") + std::to_string(pWorld->xOffset) + "\n" + std::string("Y     ") + std::to_string(pWorld->yOffset)).c_str(), RED);
	
	pFont->DrawText(Maths::IVec2D(700, 10), (std::string("FPS   : ") + std::to_string(static_cast<int>(1 / pTimer->DeltaTime()))).c_str(), GREEN);

	if (!saveTimer.IsTimerDown()) {
		text.Draw("Game  saved  !", BLACK, GRAY, WHITE);
	}
}

void ExplorationScene::SaveToJSON()
{
	pWorld->SaveToJSON();
	pPlayer->SaveToJSON();
}

void ExplorationScene::InitFromJSON()
{
	pWorld->InitFromJSON();
	pPlayer->InitFromJSON();	
}