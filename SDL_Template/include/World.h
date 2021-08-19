#pragma once

#include <random>
#include <unordered_map>
#include "Graphics.h"
#include "Player.h"
#include "NPC.h"

#include "TImerManager.h"

#include "Consumable.h"
#include "Ball.h"

class World : public CoreSystem::SingletonMaker<World> {
private:
	friend class ExplorationScene;
public:
	class Tile {
	private:
		friend class World;
	public:
		enum class GroundType
		{
			Grass = 0,
			Dirt = 1,
			Rocks = 2,
			Sand = 3,
			House0 = 4,
			House1 = 5,
			House2 = 6,
			House3 = 7,
			House4 = 8,
			House5 = 9,
			House6 = 10,
			House7 = 11,
			House8 = 12,
			House9 = 13,
			House10 = 14,
			House11 = 15,
			House12 = 16,
			House13 = 17,
			House14 = 18,
			House15 = 19,
			Water = 20,
			None = -1
		};
		enum class EventType {
			 Item,
			 Tree,
			 Boulder,
			 None
		};
		enum class BiomeType {
			Forest,
			Desert,
			Toundra,
			None
		};
	public:
		Tile() = default;
		//Generation constructor
		Tile(int x_world_pos, int y_world_pos, int seed, const World& grid);
		//Arbitrary types constructor
		Tile(GroundType g_type, EventType e_type, BiomeType b_type);

		std::shared_ptr<Item> CreateItem(std::vector<std::shared_ptr<Item>> items) const;

		void ClearEventType();

		Tile::GroundType GetGroundType() const;
		Tile::EventType GetEventType() const;
		Tile::BiomeType GetBiomeType() const;

		bool IsObstacle() const;
		bool PlayerTriggersFight(const World& grid);

		bool operator==(const Tile& rhs) const;
		bool operator!=(const Tile& rhs) const;

	private:
		void InitFromJSON(Tile::GroundType g_type, Tile::EventType e_type, Tile::BiomeType b_type);

		float PerlinNoise(float x, float y, std::vector<int> p, int nOctaves);
		Maths::FVec2D GetConstantVector(int value);
		float Fade(float t);
		float Lerp(float val0, float val1, float alpha);

	private:
		Tile::GroundType groundType = Tile::GroundType::None;
		Tile::EventType eventType = Tile::EventType::None;
		Tile::BiomeType biomeType = Tile::BiomeType::None;
	};
public:
	World();
	
	void Update(float speed);
	void Draw();

	void BlendSpriteTo(GraphicsEngine::Color c);

	bool PlayerTriggersFight();
	bool GoInside() const;

	Maths::LLVec2D GetPlayerPosition() const;
	void SetGuestPositionAndAnimation(const Maths::LLVec2D& pos, int anim);

	Uint8 GetWorldSeed() const;
	void SetWorldSeed(Uint8 seed);

	Maths::LLVec2D GetLastTileToUpdate();
	void AddTilesToUpdate(const Maths::LLVec2D& pos);

private:
	void GenerateGrid();
	void GenerateNewBiomePlaces();
	void UpdateTiles();

	uint32_t Lehmer32(uint32_t nLehmer) const;

	int rndInt(int min, int max, uint32_t nLehmer) const;

	void UpdateTempest();

	void MakePermutation();

	void CreateHouseAt(const Maths::LLVec2D& pos);

	bool TileIsObstacleAt(const Maths::LLVec2D& pos);
	bool NextTileIsWater() const;
	bool NextTileHas(World::Tile::EventType e_type) const;

	void InitFromJSON();
	void SaveToJSON();

	int GetNeighbourGroundType(const Maths::LLVec2D& pos, World::Tile::GroundType g_type) const;

	Maths::IVec2D GetPlayerDirection() const;

private:
	std::shared_ptr<NPC> pGuest;

	GraphicsEngine::Sprite tileSpriteForest;
	GraphicsEngine::Sprite tileSpriteDesert;
	GraphicsEngine::Sprite tileSpriteToundra;

	GraphicsEngine::Sprite tempestSprite;

	GraphicsEngine::Animation tempestForest;
	GraphicsEngine::Animation tempestDesert;
	GraphicsEngine::Animation tempestToundra;

	TimerManager weatherTimer;
	bool bTempestOn = false;

	bool bTilesAreWriting = false;

	std::shared_ptr<GraphicsEngine::Graphics> pGfx;
	std::shared_ptr<CoreSystem::Keyboard> pKbd;
	std::shared_ptr<CoreSystem::Timer> pTimer;
	std::shared_ptr<Player> pPlayer;

	const int tileWidth = 32;
	const int tileHeight = 32;
	const int gridHeight = 19;
	const int gridWidth = 25;


	long long lastPlayerXPos;
	long long lastPlayerYPos;
	long long currentPlayerXPos;
	long long currentPlayerYPos;

	long long xOffset = 0;
	long long yOffset = 0;
	
	Maths::IVec2D playerDirection;

	Uint8 generationSeed;
	//New version
	std::unordered_map<Maths::LLVec2D, Tile, Maths::LLVec2D::Hash> tiles;

	std::vector<Maths::LLVec2D> tilesToUpdate;
	Maths::LLVec2D lastTileToUpdate;

	//Voronoi
	std::unordered_map<Maths::LLVec2D, std::pair<Maths::LLVec2D, Tile::BiomeType>, Maths::LLVec2D::Hash> biomePlaces;

	std::vector<int> permutationArray;

	std::vector<std::shared_ptr<Item>> itemList;
	std::unordered_map<Maths::LLVec2D, std::shared_ptr<Item>, Maths::LLVec2D::Hash> items;
};