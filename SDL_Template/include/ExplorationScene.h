#pragma once
#include "Scene.h"
#include "Grid.h"
#include "Font.h"

#include "ExplorationMenu.h"
#include "PokemonInventoryMenu.h"
#include "ItemInventoryMenu.h"

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
	ExplorationScene();
	~ExplorationScene();

	void Update() override;
	void Draw() override;

private:
	void SaveToJSON();
	void InitFromJSON();

private:

	MenuState state = MenuState::None;

	int chosenItem = -1;

	bool bIsPlayingSong = false;
	bool bCurrentDayState = false;

	std::unique_ptr<AMenu> explorationMenu;
	std::unique_ptr<AMenu> itemInventoryMenu;
	std::unique_ptr<AMenu> pokemonInventoryMenu;

	std::shared_ptr<Grid> pGrid;
	std::shared_ptr<Player> pPlayer;
	std::shared_ptr<CoreSystem::Timer> pTimer;
	std::shared_ptr<CoreSystem::Mouse> pMouse;
	std::shared_ptr<CoreSystem::Window> pWnd;
	std::shared_ptr<CoreSystem::Keyboard> pKbd;
	std::shared_ptr<SoundEngine::SoundSystem> pSoundSystem;
};