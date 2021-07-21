#pragma once
#include "Scene.h"
#include "Grid.h"
#include "Font.h"

#include "TopMenu.h"
#include "ItemInventory.h"
#include "BasicMenu.h"

class ExplorationScene : public Scene
{
public:
	ExplorationScene();
	~ExplorationScene();

	void Update() override;
	void Draw() override;

private:
	void SaveToJSON();
	void InitFromJSON();

private:
	GraphicsEngine::Font font = GraphicsEngine::Font("ttf/arcadeClassic.TTF", 16);

	size_t lastPlayerItemSize = 0;

	bool bIsShowingMenu = false;
	bool bIsShowingItemInventory = false;

	std::unique_ptr<AMenu> menu;
	std::unique_ptr<AMenu> itemInventoryMenu;

	std::shared_ptr<Grid> pGrid;
	std::shared_ptr<Player> pPlayer;
	std::shared_ptr<CoreSystem::Timer> pTimer;
	std::shared_ptr<CoreSystem::Mouse> pMouse;
	std::shared_ptr<CoreSystem::Window> pWnd;
	std::shared_ptr<CoreSystem::Keyboard> pKbd;
};