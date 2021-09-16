#pragma once
#include "Scene.h"
#include "World.h"
#include "House.h"
#include "Font.h"

#include "ExplorationMenu.h"
#include "PokemonInventoryMenu.h"
#include "ItemInventoryMenu.h"

#include "TImerManager.h"
#include "SoundSystem.h"

class ExplorationScene : public Scene
{
private:
	enum class MenuState {
		ShowingMenu,
		ShowingItemInventory,
		ShowingPokemonInventory,
		HealingPokemon,
		None
	};
public:
	ExplorationScene(std::shared_ptr<Player> pPlayer);

	void Update() override;
	void Draw() override;

private:
	void SaveToJSON();
	void InitFromJSON();

private:

	MenuState state = MenuState::None;

	int chosenItem = -1;

	bool bHasInitOnline = false;
	bool TEST_bIsConnected = false;

	bool bIsPlayingSong = false;
	bool bCurrentDayState = false;
	bool bIsInsideHouse = false;
	bool bInTransition = false;

	std::unique_ptr<AMenu> explorationMenu;
	std::unique_ptr<AMenu> itemInventoryMenu;
	std::unique_ptr<AMenu> pokemonInventoryMenu;

	House house;
	std::shared_ptr<World> pWorld;

	TimerManager transitionTimer;
	TimerManager saveTimer;

	TextBox text;
};