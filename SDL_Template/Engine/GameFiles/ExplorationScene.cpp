#include "ExplorationScene.h"

ExplorationScene::ExplorationScene()
	:
	Scene(Scene::SceneType::ExplorationScene),
	menu(std::make_unique<TopMenu>(std::make_unique<BasicMenu>())),
	itemInventoryMenu(std::make_unique<ItemInventory>(std::make_unique<BasicMenu>())),
	pokemonInventoryMenu(std::make_unique<PokemonInventory>(std::make_unique<BasicMenu>())),
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
	itemInventoryMenu = std::make_unique<ItemInventory>(std::make_unique<BasicMenu>());
	pokemonInventoryMenu = std::make_unique<PokemonInventory>(std::make_unique<BasicMenu>());

	//Init from json
	if (pPlayer->TEST_bInitFromJSON) {
		pPlayer->TEST_bInitFromJSON = false;
		InitFromJSON();
	}

	bWillChangeScene = false;

	pPlayer->Update(pTimer->DeltaTime());
	if (!bIsShowingMenu && !bIsShowingItemInventory && !bIsShowingPokemonInventory) {
		pPlayer->Move();
		pGrid->Update();
	}
	else {
		if (bIsShowingMenu) {
			int output = -1;
			menu->Update(output, pMouse);

			switch (output) {
			case 0:
				pWnd->ExitGame();
				break;
			case 1:
				bIsShowingItemInventory = true;
				bIsShowingMenu = false;
				break;
			case 2:
				bIsShowingPokemonInventory = true;
				bIsShowingMenu = false;
				break;
			case 3:
				pPlayer->SaveJSON();
				SaveToJSON();
				break;
			case 4:
				bWillChangeScene = true;
				bIsShowingMenu = false;
				bIsPlayingSong = false;
				pSoundSystem->StopSounds();
				newScene = Scene::SceneType::TitleScene;
				break;
			default:
				break;
			}
		}
		if (bIsShowingItemInventory) {
				int output = -1;
				itemInventoryMenu->Update(output, pMouse);
				if (output != -1) {
					pPlayer->TEST_UseItem(output);
					printf("item clicked!\n");
				}
			}
		if (bIsShowingPokemonInventory) {
			int output = -1;
			pokemonInventoryMenu->Update(output, pMouse);
			if (output != -1) {
				printf("Pokemon clicked!\n");
			}
		}
	}

	if (pGrid->PlayerTriggersFight() && !pPlayer->GetPokemon().IsDead()) {
		bWillChangeScene = true;
		bIsShowingMenu = false;
		bIsPlayingSong = false;
		pSoundSystem->StopSounds();
		newScene = Scene::SceneType::FightingScene;
	}

	auto e = pKbd->ReadKey();

	if (e.keycode == SDL_SCANCODE_ESCAPE && e.type == CoreSystem::Keyboard::Event::Type::Pressed) {
		bIsShowingMenu = !bIsShowingMenu;
		if (bIsShowingItemInventory) {
			bIsShowingItemInventory = false;
		}
		else if (bIsShowingPokemonInventory) {
			bIsShowingPokemonInventory = false;
		}
	}
}

void ExplorationScene::Draw()
{
	pGrid->Draw();
	pPlayer->Draw();
	//pPlayer->DrawPokemon();

	if (bIsShowingMenu) {
		menu->Draw();
	}
	if (bIsShowingItemInventory) {
		itemInventoryMenu->Draw();
	}
	if (bIsShowingPokemonInventory) {
		pokemonInventoryMenu->Draw();
	}
	pFont->DrawText(Maths::IVec2D(10, 10), (std::string("X     ") + std::to_string(pGrid->GetWorldPosition().x) + "\n" + std::string("Y     ") + std::to_string(pGrid->GetWorldPosition().y)).c_str(), RED);
}

void ExplorationScene::SaveToJSON()
{
	JSONParser::Writer jsonWriter;

	jsonWriter.AddValueForMember("Player", pGrid->GetWorldPosition().x, pGrid->GetWorldPosition().y);

	for (auto& entry : pGrid->items) {
		std::shared_ptr<Consumable> consumable = std::dynamic_pointer_cast<Consumable, Item>(entry.second);
		if (consumable != nullptr) {
			jsonWriter.AddObjectMember("Consumables", consumable->GetName(), entry.first.x, entry.first.y, consumable->GetID(), consumable->GetBonusValue());
		}
		else {
			std::shared_ptr<Ball> ball = std::dynamic_pointer_cast<Ball, Item>(entry.second);
			if (ball != nullptr) {
				jsonWriter.AddObjectMember("Balls", ball->GetName(), entry.first.x, entry.first.y, ball->GetID(), ball->GetProbability());
			}
		}
	}

	jsonWriter.SaveJsonAt("json/mapCoords.json");

	pPlayer->SaveJSON();
}

void ExplorationScene::InitFromJSON()
{
	JSONParser::Reader jsonReader;
	jsonReader.ReadFile("json/mapCoords.json");

	std::unordered_map<Maths::IVec2D, std::shared_ptr<Item>, Maths::IVec2D::Hash> itemsFromJSON;

	//Init consumables
	if (jsonReader.IsValueAvailable("Consumables")) {
		auto& consumables = jsonReader.GetValueOf("Consumables");
		for (auto itr = consumables.MemberBegin(); itr != consumables.MemberEnd(); ++itr) {
			itemsFromJSON.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(Maths::IVec2D(itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt()), std::make_shared<Consumable>(itr->name.GetString(), itr->value.GetArray()[2].GetInt(), itr->value.GetArray()[3].GetInt())));
		}
	}

	//Init balls
	if (jsonReader.IsValueAvailable("Balls")) {
		auto& balls = jsonReader.GetValueOf("Balls");
		for (auto itr = balls.MemberBegin(); itr != balls.MemberEnd(); ++itr) {
			itemsFromJSON.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(Maths::IVec2D(itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt()), std::make_shared<Ball>(itr->name.GetString(), itr->value.GetArray()[2].GetInt(), itr->value.GetArray()[3].GetInt())));
		}
	}

	auto& value = jsonReader.GetValueOf("Player");
	pGrid->SetWorldPosition(Maths::IVec2D(value.GetArray()[0].GetInt(), value.GetArray()[1].GetInt()));

	pGrid->items.clear();
	pGrid->items = itemsFromJSON;
	
	pPlayer->InitFromJSON();
}