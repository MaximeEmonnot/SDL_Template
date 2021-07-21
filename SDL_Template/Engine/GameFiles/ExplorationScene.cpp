#include "ExplorationScene.h"

ExplorationScene::ExplorationScene()
	:
	Scene(Scene::SceneType::ExplorationScene),
	menu(std::make_unique<TopMenu>(std::make_unique<BasicMenu>())),
	itemInventoryMenu(std::make_unique<ItemInventory>(std::make_unique<BasicMenu>())),
	pGrid(Grid::GetInstance()),
	pPlayer(Player::GetInstance(Maths::IRect(384, 284, 32, 32), "json/kirby.json")),
	pTimer(CoreSystem::Timer::GetInstance()),
	pMouse(CoreSystem::Mouse::GetInstance()),
	pWnd(CoreSystem::Window::GetInstance()),
	pKbd(CoreSystem::Keyboard::GetInstance())
{
}

ExplorationScene::~ExplorationScene()
{
	pGrid->Kill();
}

void ExplorationScene::Update()
{
	if (pPlayer->GetItemList().size() != lastPlayerItemSize) {
		itemInventoryMenu = nullptr;
		itemInventoryMenu = std::make_unique<ItemInventory>(std::make_unique<BasicMenu>());
		lastPlayerItemSize = pPlayer->GetItemList().size();
	}
	if (pPlayer->TEST_bInitFromJSON) {
		pPlayer->TEST_bInitFromJSON = false;
		InitFromJSON();
	}

	bWillChangeScene = false;

	pPlayer->Update(pTimer->DeltaTime());
	if (!bIsShowingMenu && !bIsShowingItemInventory) {
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
				break;
			case 3:
				pPlayer->SaveJSON();
				SaveToJSON();
				break;
			case 4:
				bWillChangeScene = true;
				bIsShowingMenu = false;
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
	}

	if (pGrid->PlayerTriggersFight() && !pPlayer->GetPokemon().IsDead()) {
		bWillChangeScene = true;
		newScene = Scene::SceneType::FightingScene;
	}

	auto e = pKbd->ReadKey();

	if (e.keycode == SDL_SCANCODE_ESCAPE && e.type == CoreSystem::Keyboard::Event::Type::Pressed) {
		bIsShowingMenu = !bIsShowingMenu;
		if (bIsShowingItemInventory) {
			bIsShowingItemInventory = false;
		}
	}
}

void ExplorationScene::Draw()
{
	pGrid->Draw();
	pPlayer->Draw();
	pPlayer->DrawPokemon();

	if (bIsShowingMenu) {
		menu->Draw(font);
	}
	if (bIsShowingItemInventory) {
		itemInventoryMenu->Draw(font);
	}
	font.DrawText(Maths::IVec2D(10, 10), (std::string("X     ") + std::to_string(pGrid->GetWorldPosition().x) + "\n" + std::string("Y     ") + std::to_string(pGrid->GetWorldPosition().y)).c_str(), RED);
}

void ExplorationScene::SaveToJSON()
{
	JSONParser::Writer jsonWriter;

	jsonWriter.AddValueForMember("Player", pGrid->GetWorldPosition().x, pGrid->GetWorldPosition().y);

	for (auto& entry : pGrid->items) {
		jsonWriter.AddObjectMember("Items", entry.second, entry.first.x, entry.first.y);
	}

	jsonWriter.SaveJsonAt("json/mapCoords.json");

	pPlayer->SaveJSON();
}

void ExplorationScene::InitFromJSON()
{
	JSONParser::Reader jsonReader;
	jsonReader.ReadFile("json/mapCoords.json");

	std::unordered_map < Maths::IVec2D, std::string, Maths::IVec2D::Hash> itemsFromJSON;

	auto& object = jsonReader.GetValueOf("Items");
	for (auto itr = object.MemberBegin(); itr != object.MemberEnd(); ++itr) {
		itemsFromJSON.insert(std::pair<Maths::IVec2D, std::string>(Maths::IVec2D(itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt()), itr->name.GetString()));
	}

	auto& value = jsonReader.GetValueOf("Player");
	pGrid->SetWorldPosition(Maths::IVec2D(value.GetArray()[0].GetInt(), value.GetArray()[1].GetInt()));

	pGrid->items.clear();
	pGrid->items = itemsFromJSON;
	
	pPlayer->InitFromJSON();
}
