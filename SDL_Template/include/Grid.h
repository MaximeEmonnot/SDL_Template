#pragma once

#include <random>
#include <unordered_map>
#include "Graphics.h"
#include "Player.h"

#include "Consumable.h"
#include "Ball.h"

class Grid : public CoreSystem::SingletonMaker<Grid> {
private:
	friend class ExplorationScene;
private:
	class Tile {
	private:
		friend class Grid;
	public:
		enum class GroundType
		{
			Grass,
			Rocks,
			Sand,
			None
		};
		enum class EventType {
			 Item,
			 Tree,
			 Boulder,
			 None
		};
	public:
		Tile() = default;
		Tile(int x_world_pos, int y_world_pos, int seed);

		std::shared_ptr<Item> CreateItem(std::vector<std::shared_ptr<Item>> items);

		void ClearEventType();

		Tile::GroundType GetGroundType() const;
		Tile::EventType GetEventType() const;
		Maths::IVec2D GetWorldPosition() const;

		bool PlayerTriggersFight(int player_x_pos, int player_y_pos);
	private:
		void InitFromJSON(int x_pos, int y_pos, Tile::GroundType g_type, Tile::EventType e_type);

		uint32_t Lehmer32();

		int rndInt(int min, int max);

	private:
		uint32_t nLehmer = 0;

		Maths::IVec2D worldPos;
		Tile::GroundType groundType = Tile::GroundType::None;
		Tile::EventType eventType = Tile::EventType::None;
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

	bool TileIsObstacleAt(const Maths::IVec2D& pos);

	void InitFromJSON();
	void SaveToJSON();


private:
	GraphicsEngine::Sprite tileSprite;
	std::shared_ptr<GraphicsEngine::Graphics> pGfx;
	std::shared_ptr<CoreSystem::Keyboard> pKbd;
	std::shared_ptr<CoreSystem::Timer> pTimer;
	std::shared_ptr<Player> pPlayer;

	const int tileWidth = 32;
	const int tileHeight = 32;
	const int gridHeight = 19;
	const int gridWidth = 25;

	int lastPlayerXPos;
	int lastPlayerYPos;
	int currentPlayerXPos;
	int currentPlayerYPos;

	long long xOffset = 0;
	long long yOffset = 0;
	
	int generationSeed;
	//New version
	std::unordered_map<Maths::IVec2D, Tile, Maths::IVec2D::Hash> tiles;

	std::vector<std::shared_ptr<Item>> itemList;
	std::unordered_map<Maths::IVec2D, std::shared_ptr<Item>, Maths::IVec2D::Hash> items;
};