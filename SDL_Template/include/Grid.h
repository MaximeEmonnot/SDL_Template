#pragma once

#include <random>
#include <unordered_map>
#include "Graphics.h"
#include "Player.h"

class Grid : public CoreSystem::SingletonMaker<Grid> {
private:
	friend class ExplorationScene;
private:
	class Tile {
	public:
		enum class Type
		{
			Grass,
			Rocks,
			Sand,
			None
		};
	public:
		Tile(int x_world_pos, int y_world_pos, int seed);

		Tile::Type GetType();
		Maths::IVec2D GetWorldPosition();

		bool PlayerTriggersFight(int player_x_pos, int player_y_pos);
	private:
		uint32_t Lehmer32();

		int rndInt(int min, int max);

	private:
		uint32_t nLehmer = 0;

		Maths::IVec2D worldPos;
		Tile::Type type = Tile::Type::None;
	};
public:
	Grid();
	
	void Update();
	void Draw();

	bool PlayerTriggersFight();

	void SetWorldPosition(Maths::IVec2D pos);
	Maths::IVec2D GetWorldPosition();

private:
	void GenerateGrid();
	void GenerateItems();

	bool NextTileIsRocks(const Maths::IVec2D& pos);

private:
	GraphicsEngine::Sprite tileSprite;
	std::shared_ptr<GraphicsEngine::Graphics> pGfx;
	std::shared_ptr<CoreSystem::Keyboard> pKbd;
	std::shared_ptr<Player> pPlayer;

	const int tileWidth = 32;
	const int tileHeight = 32;
	const int gridHeight = 19;
	const int gridWidth = 25;

	int lastPlayerXPos;
	int lastPlayerYPos;
	int currentPlayerXPos;
	int currentPlayerYPos;

	int xOffset = 0;
	int yOffset = 0;
	
	int generationSeed;
	std::vector<Tile> tiles;

	std::unordered_map<Maths::IVec2D, std::string, Maths::IVec2D::Hash> items;
};