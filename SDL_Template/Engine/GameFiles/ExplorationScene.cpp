#include "ExplorationScene.h"
#include <iostream>

ExplorationScene::ExplorationScene(std::shared_ptr<Player> pPlayer)
	:
	Scene(pPlayer, Scene::SceneType::ExplorationScene),
	house(pPlayer),
	pWorld(std::make_shared<World>(pPlayer)),
	explorationMenu(std::make_unique<ExplorationMenu>(std::make_unique<BasicMenu>())),
	itemInventoryMenu(std::make_unique<ItemInventoryMenu>(std::make_unique<BasicMenu>(), pPlayer)),
	pokemonInventoryMenu(std::make_unique<PokemonInventoryMenu>(std::make_unique<BasicMenu>(), pPlayer)),
	text(Maths::IRect(25, 500, 200, 75))
{
}

void ExplorationScene::Update()
{
	//Song
	if (!bIsPlayingSong) {
		if (CoreSystem::Timer::GetInstance().IsNightTime()) {
			SoundEngine::SoundSystem::GetInstance().PlaySound(SoundEngine::SoundSystem::GetInstance().ConstructNewSong("music/nightwalk.wav", MUSIC), -1);
		}
		else {
			SoundEngine::SoundSystem::GetInstance().PlaySound(SoundEngine::SoundSystem::GetInstance().ConstructNewSong("music/daywalk.wav", MUSIC), -1);
		}
		bIsPlayingSong = true;
	}
	else {
		if (bCurrentDayState != CoreSystem::Timer::GetInstance().IsNightTime()) {
			SoundEngine::SoundSystem::GetInstance().StopSounds();
			bCurrentDayState = CoreSystem::Timer::GetInstance().IsNightTime();
			bIsPlayingSong = false;
		}
	}

	//Update Timers
	transitionTimer.Update();
	saveTimer.Update();

	//Update menus
	itemInventoryMenu = nullptr;
	pokemonInventoryMenu = nullptr;
	itemInventoryMenu = std::make_unique<ItemInventoryMenu>(std::make_unique<BasicMenu>(), pPlayer);
	pokemonInventoryMenu = std::make_unique<PokemonInventoryMenu>(std::make_unique<BasicMenu>(), pPlayer);

	//Init from json
	if (pPlayer->TEST_bInitFromJSON) {
		pPlayer->TEST_bInitFromJSON = false;
		InitFromJSON();
	}

	//Init online
	/*
	if (pPlayer->IsOnline() && !bHasInitOnline)
	{
		bHasInitOnline = true;
		//Host player online routine
		if (pPlayer->bIsHost) {
			//Send
			CoreSystem::ThreadPool::GetInstance(30).Enqueue([&] {
				int64_t packetIndex = 0;
				while (CoreSystem::Window::GetInstance().ListensToEvents()) {
					if (!TEST_bIsConnected) {
						long long xPlayerPos = pWorld->GetPlayerPosition().x;
						long long yPlayerPos = pWorld->GetPlayerPosition().y;
						Maths::LLVec2D lastTileToUpdate = pWorld->GetLastTileToUpdate();
						std::vector<Uint8> dataOut;

						//World seed packaging
						dataOut.push_back(pWorld->GetWorldSeed());

						//PacketIndex data packaging
						int64_t index = packetIndex;
						for (int i = 0; i < 8; i++) {
							dataOut.push_back(static_cast<Uint8>(index >> 56));
							index <<= 8;
						}

						//xPlayerPos data packaging
						for (int i = 0; i < 8; i++) {
							dataOut.push_back(static_cast<Uint8>(xPlayerPos >> 56));
							xPlayerPos <<= 8;
						}
						//yPlayerPos data packaging
						for (int i = 0; i < 8; i++) {
							dataOut.push_back(static_cast<Uint8>(yPlayerPos >> 56));
							yPlayerPos <<= 8;
						}
						//playerAnimation data packaging
						dataOut.push_back(static_cast<Uint8>(pPlayer->GetAnimation()));
						//playerLocomotionState data packaging
						dataOut.push_back(static_cast<Uint8>(pPlayer->GetLocomotionState()));

						//xLastTileToUpdate data packaging
						if (lastTileToUpdate != Maths::LLVec2D(0, 0)) {
							for (int i = 0; i < 8; i++) {
								dataOut.push_back(static_cast<Uint8>(lastTileToUpdate.x >> 56));
								lastTileToUpdate.x <<= 8;
							}
							//yLastTileToUpdate data packaging
							for (int i = 0; i < 8; i++) {
								dataOut.push_back(static_cast<Uint8>(lastTileToUpdate.y >> 56));
								lastTileToUpdate.y <<= 8;
							}
						}
						Network::NetworkSystem::GetInstance("127.0.0.1", 333, 222).SendPackage(dataOut);
						packetIndex++;
					}
				}
				});
			//Recieve
			CoreSystem::ThreadPool::GetInstance(30).Enqueue([&] {
				int64_t lastPacketIndex = 0;
				while (CoreSystem::Window::GetInstance().ListensToEvents()) {
					std::vector<Uint8> dataIn = Network::NetworkSystem::GetInstance("127.0.0.1", 333, 222).RecievePackage();

					if (!TEST_bIsConnected) {

						//PacketIndexReading
						int64_t index = 0;
						for (size_t i = 0; i < 8; i++) {
							index <<= 8;
							index += dataIn.at(i);
						}
						if (index >= lastPacketIndex) {
							lastPacketIndex = index;
							//xGuestPos reading
							long long xGuestPos = 0;
							for (size_t i = 8; i < 16; i++) {
								xGuestPos <<= 8;
								xGuestPos += dataIn.at(i);
							}
							//yGuestPos reading
							long long yGuestPos = 0;
							for (size_t i = 16; i < 24; i++) {
								yGuestPos <<= 8;
								yGuestPos += dataIn.at(i);
							}
							//Set Guest Position in World
							pWorld->SetGuestPositionAndAnimation(Maths::LLVec2D(xGuestPos, yGuestPos), static_cast<int>(dataIn.at(24)), static_cast<int>(dataIn.at(25)));

							if (dataIn.size() > 26) {
								//xLastTileToUpdatePos reading
								long long xLastTileToUpdate = 0;
								for (size_t i = 26; i < 34; i++) {
									xLastTileToUpdate <<= 8;
									xLastTileToUpdate += dataIn.at(i);
								}

								//yLastTileToUpdatePos reading
								long long yLastTileToUpdate = 0;
								for (size_t i = 34; i < 42; i++) {
									yLastTileToUpdate <<= 8;
									yLastTileToUpdate += dataIn.at(i);
								}

								//Add Last tile to update to Tiles to Update list
								pWorld->AddTilesToUpdate(Maths::LLVec2D(xLastTileToUpdate, yLastTileToUpdate));
							}
						}
					}
				}
				});
		}
		//Guest player online routine
		else {
			//Send
			CoreSystem::ThreadPool::GetInstance(30).Enqueue([&] {
				int64_t packetIndex = 0;
				while (CoreSystem::Window::GetInstance().ListensToEvents()) {
					if (!TEST_bIsConnected) {
						long long xPlayerPos = pWorld->GetPlayerPosition().x;
						long long yPlayerPos = pWorld->GetPlayerPosition().y;
						Maths::LLVec2D lastTileToUpdate = pWorld->GetLastTileToUpdate();
						std::vector<Uint8> dataOut;

						//PacketIndex data packaging
						int64_t index = packetIndex;
						for (int i = 0; i < 8; i++) {
							dataOut.push_back(static_cast<Uint8>(index >> 56));
							index <<= 8;
						}

						//xPlayerPos data packaging
						for (int i = 0; i < 8; i++) {
							dataOut.push_back(static_cast<Uint8>(xPlayerPos >> 56));
							xPlayerPos <<= 8;
						}
						//yPlayerPos data packaging
						for (int i = 0; i < 8; i++) {
							dataOut.push_back(static_cast<Uint8>(yPlayerPos >> 56));
							yPlayerPos <<= 8;
						}
						//playerAnimation data packaging
						dataOut.push_back(static_cast<Uint8>(pPlayer->GetAnimation()));
						//playerLocomotionState data packaging
						dataOut.push_back(static_cast<Uint8>(pPlayer->GetLocomotionState()));

						if (lastTileToUpdate != Maths::LLVec2D(0, 0)) {
							//xLastTileToUpdate data packaging
							for (int i = 0; i < 8; i++) {
								dataOut.push_back(static_cast<Uint8>(lastTileToUpdate.x >> 56));
								lastTileToUpdate.x <<= 8;
							}
							//yLastTileToUpdate data packaging
							for (int i = 0; i < 8; i++) {
								dataOut.push_back(static_cast<Uint8>(lastTileToUpdate.y >> 56));
								lastTileToUpdate.y <<= 8;
							}
						}

						Network::NetworkSystem::GetInstance("127.0.0.1", 222, 333).SendPackage(dataOut);
						packetIndex++;
					}
				}
				});
			//Recieve
			CoreSystem::ThreadPool::GetInstance(30).Enqueue([&] {
				int64_t lastPacketIndex = 0;
				std::vector<Uint8> dataSeed = Network::NetworkSystem::GetInstance("127.0.0.1", 222, 333).RecievePackage();

				//Set World Seed
				pWorld->SetWorldSeed(dataSeed.at(0));

				while (CoreSystem::Window::GetInstance().ListensToEvents()) {
					std::vector<Uint8> dataIn = Network::NetworkSystem::GetInstance("127.0.0.1", 222, 333).RecievePackage();

					if (!TEST_bIsConnected) {
						//PacketIndexReading
						int64_t index = 0;
						for (size_t i = 1; i < 9; i++) {
							index <<= 8;
							index += dataIn.at(i);
						}
						if (index >= lastPacketIndex) {
							lastPacketIndex = index;
							//xGuestPos reading
							long long xGuestPos = 0;
							for (size_t i = 9; i < 17; i++) {
								xGuestPos <<= 8;
								xGuestPos += dataIn.at(i);
							}

							//yGuestPos reading
							long long yGuestPos = 0;
							for (size_t i = 17; i < 25; i++) {
								yGuestPos <<= 8;
								yGuestPos += dataIn.at(i);
							}

							//Set Guest Position in World
							pWorld->SetGuestPositionAndAnimation(Maths::LLVec2D(xGuestPos, yGuestPos), static_cast<int>(dataIn.at(25)), static_cast<int>(dataIn.at(26)));

							if (dataIn.size() > 27) {
								//xLastTileToUpdatePos reading
								long long xLastTileToUpdate = 0;
								for (size_t i = 27; i < 35; i++) {
									xLastTileToUpdate <<= 8;
									xLastTileToUpdate += dataIn.at(i);
								}

								//yLastTileToUpdatePos reading
								long long yLastTileToUpdate = 0;
								for (size_t i = 35; i < 43; i++) {
									yLastTileToUpdate <<= 8;
									yLastTileToUpdate += dataIn.at(i);
								}

								//Add Last tile to update to Tiles to Update list
								pWorld->AddTilesToUpdate(Maths::LLVec2D(xLastTileToUpdate, yLastTileToUpdate));
							}
						}
					}
				}
				});
		}
	}
	*/

	bWillChangeScene = false;

	pPlayer->Update(CoreSystem::Timer::GetInstance().DeltaTime());

	if (TEST_bIsConnected) {
		auto key = CoreSystem::Keyboard::GetInstance().ReadKey();
		if (!pPlayer->bIsHost) {
			if (key.keycode == SDL_SCANCODE_L && key.type == CoreSystem::Keyboard::Event::Type::Pressed) {
				std::vector<Uint8> testDataOut;
				testDataOut.push_back(69);
				testDataOut.push_back(420);
				testDataOut.push_back(69);
				Network::NetworkSystem::GetInstance().SendDataTCP(testDataOut);
				std::vector<Uint8> testDataIn = Network::NetworkSystem::GetInstance().RecieveDataTCP();
				for (auto& data : testDataIn) {
					std::cout << std::to_string(data) << "\n";
				}
			}
		}
		else {
			if (key.keycode == SDL_SCANCODE_L && key.type == CoreSystem::Keyboard::Event::Type::Pressed) {
				std::vector<Uint8> testDataOut;
				testDataOut.push_back(69);
				testDataOut.push_back(420);
				testDataOut.push_back(69);
				testDataOut.push_back(69);
				testDataOut.push_back(69);
				testDataOut.push_back(420);
				Network::NetworkSystem::GetInstance().SendDataTCP(testDataOut);
				std::vector<Uint8> testDataIn = Network::NetworkSystem::GetInstance().RecieveDataTCP();
				for (auto& data : testDataIn) {
					std::cout << std::to_string(data) << "\n";
				}
			}
		}
	}
	else {
		if (pPlayer->bIsOnline && CoreSystem::Keyboard::GetInstance().KeyIsPressed(SDL_SCANCODE_P)) {
			TEST_bIsConnected = true;
			if (pPlayer->bIsHost) {
				Network::NetworkSystem::GetInstance().TCPWaitForClient();
			}
			else {
				Network::NetworkSystem::GetInstance().TCPJoinHost();
			}
		}
	}

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
		explorationMenu->Update(output);
		switch (output) {
		case 0:
			CoreSystem::Window::GetInstance().ExitGame();
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
			pPlayer->ConnectAs(SERVER);
			state = MenuState::None;
			break;
		case 5:
			saveTimer.ResetTimer(2.5f);
			pPlayer->SaveToJSON();
			SaveToJSON();
			break;
		case 6:
			bWillChangeScene = true;
			state = MenuState::None;
			bIsPlayingSong = false;
			SoundEngine::SoundSystem::GetInstance().StopSounds();
			newScene = Scene::SceneType::TitleScene;
			break;
		default:
			break;
		}
		break;
	case MenuState::ShowingItemInventory:
		itemInventoryMenu->Update(output);
		if (output != -1) {
			if (pPlayer->CanUseItem(output)) {
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
		pokemonInventoryMenu->Update(output);
		if (output != -1) {
			pPlayer->UsePokemon(output);
			state = MenuState::None;
			printf("Pokemon clicked!\n");
		}
		break;
	case MenuState::HealingPokemon:
		pokemonInventoryMenu->Update(output);
		if (output != -1) {
			pPlayer->UseItem(chosenItem, output);
			state = MenuState::None;
			printf("Pokemon healed !");
		}
	default:
		break;
	}

	if (pWorld->PlayerTriggersFight() && !pPlayer->GetPokemon().IsDead()) {
		bWillChangeScene = true;
		bIsPlayingSong = false;
		SoundEngine::SoundSystem::GetInstance().StopSounds();
		newScene = Scene::SceneType::FightingScene;
	}

	auto e = CoreSystem::Keyboard::GetInstance().ReadKey();

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
		if (CoreSystem::Timer::GetInstance().IsNightTime())
		{
			pWorld->BlendSpriteTo(GraphicsEngine::Color(64, 64, 128, 128));
			pPlayer->BlendSpriteTo(GraphicsEngine::Color(64, 64, 128, 128));
		}
		pWorld->Draw();
		pPlayer->DrawReflection();
	}
	pPlayer->Draw();

	if (bInTransition) {
		GraphicsEngine::Graphics::GetInstance().FadeOutScreen(transitionTimer.GetTimer() / 1.5f);
	}
	else if (!transitionTimer.IsTimerDown()) {
		GraphicsEngine::Graphics::GetInstance().FadeInScreen(transitionTimer.GetTimer() / 1.5f);
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
	GraphicsEngine::Font::GetInstance("ttf/arcardeClassic.ttf", 16).DrawText(Maths::IVec2D(10, 10), (std::string("X     ") + std::to_string(pWorld->xOffset) + "\n" + std::string("Y     ") + std::to_string(pWorld->yOffset)).c_str(), RED);
	
	GraphicsEngine::Font::GetInstance("ttf/arcardeClassic.ttf", 16).DrawText(Maths::IVec2D(700, 10), (std::string("FPS   : ") + std::to_string(static_cast<int>(1 / CoreSystem::Timer::GetInstance().DeltaTime()))).c_str(), GREEN);

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