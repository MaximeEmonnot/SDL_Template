#include "World.h"
#include <iostream>

World::Tile::Tile(int x_world_pos, int y_world_pos, int seed, const World& grid)
{
	//Biome Determination
	std::pair<Maths::LLVec2D, Tile::BiomeType> nearestPoint;
	for (auto& biomePoint : grid.biomePlaces) {
		int smalledDist = 100000;
		int manhattanDistPoint = static_cast<int>(abs(biomePoint.second.first.x - x_world_pos) + abs(biomePoint.second.first.y - y_world_pos));
		if (smalledDist > manhattanDistPoint) {
			smalledDist = manhattanDistPoint;
			nearestPoint = biomePoint.second;
		}
	}
	biomeType = nearestPoint.second;

	if (x_world_pos <= 0 || y_world_pos <= 0) {
		groundType = GroundType::Rocks;
	}
	else {
		uint32_t nLehmer = (x_world_pos & 0xFFFF) << seed | (y_world_pos & 0xFFFF);

		int probaGrass = 0;
		int probaRocks = 0;

		Maths::LLVec2D worldPos = Maths::LLVec2D(x_world_pos, y_world_pos);
		float perlin = PerlinNoise(x_world_pos * 0.1f, y_world_pos * 0.1f, grid.permutationArray, 3);

		if (perlin > 0.0f || (x_world_pos < 15 && y_world_pos < 15)) {
			switch (grid.GetNeighbourGroundType(worldPos, Tile::GroundType::Grass)) {
			case 1:
				probaGrass = 20;
			case 2:
			case 3:
				probaGrass = 40;
				break;
			case 4:
			case 5:
				probaGrass = 55;
				break;
			case 6:
			case 7:
				probaGrass = 75;
				break;
			case 8:
				probaGrass = 80;
			default:
				break;
			}
			switch (grid.GetNeighbourGroundType(worldPos, Tile::GroundType::Rocks))
			{
			case 1:
			case 2:
				probaRocks = 15;
			case 3:
			case 4:
			case 5:
				probaRocks = 10;
			case 6:
			case 7:
			case 8:
				probaRocks = 5;
			default:
				break;
			}
			if (grid.rndInt(0, 1000, nLehmer) < 1) {
				if (grid.GetPlayerDirection().x >= 0) {
					groundType = Tile::GroundType::House0;
				}
				else {
					groundType = Tile::GroundType::House3;
				}

			}
			else {
				if (grid.rndInt(0, 100, nLehmer) < probaRocks) {
					groundType = GroundType::Rocks;
				}
				else if (grid.rndInt(0, 100, nLehmer) < probaGrass) {
					groundType = GroundType::Grass;
					if (grid.rndInt(0, 200, nLehmer) < 1) {
						eventType = EventType::Item;
					}
				}
				else {
					// 50% grass 25% Forest 15% water 8% Rocks 2% Sand
					if (grid.rndInt(0, 10, nLehmer) < 8) {
						groundType = GroundType::Dirt;

						if (grid.rndInt(0, 100, nLehmer) < 1) {
							eventType = EventType::Tree;
						}
						else if (grid.rndInt(0, 125, nLehmer) < 1) {
							eventType = EventType::Item;
						}
						else if (grid.rndInt(0, 150, nLehmer) < 1) {
							eventType = EventType::Boulder;
						}
					}
					else {
						if (grid.rndInt(0, 4, nLehmer) < 3) {
							groundType = GroundType::Grass;
							if (grid.rndInt(0, 250, nLehmer) < 1) {
								eventType = EventType::Item;
							}
						}
						else {
							groundType = GroundType::Rocks;
						}
					}
				}
			}
		}
		else if (x_world_pos >= 15 || y_world_pos >= 15) {
			if (perlin > -0.15f) {
				groundType = GroundType::Sand;
				if (grid.rndInt(0, 125, nLehmer) < 1) {
					eventType = EventType::Boulder;
				}
			}
			else {
				groundType = GroundType::Water;
			}
		}
	}
}

World::Tile::Tile(GroundType g_type, EventType e_type, BiomeType b_type)
	:
	groundType(g_type),
	eventType(e_type),
	biomeType(b_type)
{
}

std::shared_ptr<Item> World::Tile::CreateItem(std::vector<std::shared_ptr<Item>> items) const
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(0, 100);

	if (dist(rng) < 40) {//Consumables
		if (dist(rng) < 35) return items.at(0); // Potions
		else if (dist(rng) < 55) return items.at(1); // Super Potions
		else if (dist(rng) < 70) return items.at(2); // Hyper Potions
		else if (dist(rng) < 73) return items.at(3); // Potion Max
		else if (dist(rng) < 83) return items.at(4); // Attack+
		else if (dist(rng) < 93) return items.at(5); // Defense+
		else if (dist(rng) < 98) return items.at(6); // HP+
		else return items.at(7); // Super Candy
	}
	else {
		if (dist(rng) < 45) return items.at(8); // Pokeball
		else if (dist(rng) < 78) return items.at(9); // Superball
		else if (dist(rng) < 99) return items.at(10); // Hyperball
		else return items.at(11); // Masterball
	}
}

void World::Tile::ClearEventType()
{
	eventType = World::Tile::EventType::None;
}

World::Tile::EventType World::Tile::GetEventType() const
{
	return eventType;
}

World::Tile::BiomeType World::Tile::GetBiomeType() const
{
	return biomeType;
}

World::Tile::GroundType World::Tile::GetGroundType() const
{
	return groundType;
}

bool World::Tile::IsObstacle() const
{
	switch (groundType)
	{
	case World::Tile::GroundType::Rocks:
	case World::Tile::GroundType::Water:
	case World::Tile::GroundType::House0:
	case World::Tile::GroundType::House1:
	case World::Tile::GroundType::House2:
	case World::Tile::GroundType::House3:
	case World::Tile::GroundType::House4:
	case World::Tile::GroundType::House5:
	case World::Tile::GroundType::House6:
	case World::Tile::GroundType::House7:
	case World::Tile::GroundType::House8:
	case World::Tile::GroundType::House9:
	case World::Tile::GroundType::House10:
	case World::Tile::GroundType::House11:
	case World::Tile::GroundType::House12:
	case World::Tile::GroundType::House13:
	case World::Tile::GroundType::House14:
	case World::Tile::GroundType::House15:
		return true;
		break;
	default:
		break;
	}

	return eventType != EventType::None;
}

bool World::Tile::PlayerTriggersFight(const World& grid)
{
	if (groundType == Tile::GroundType::Grass) {
		std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<int> dist(0, 20);
		if (grid.pTimer->IsNightTime()) {
			return (dist(rng) <= 4);
		}
		else {
			return (dist(rng) == 1);
		}
	}
	return false;
}

bool World::Tile::operator==(const Tile& rhs) const
{
	return groundType == rhs.groundType && eventType == rhs.eventType && biomeType == rhs.biomeType;
}

bool World::Tile::operator!=(const Tile& rhs) const
{
	return !(*this == rhs);
}

void World::Tile::InitFromJSON(Tile::GroundType g_type, Tile::EventType e_type, Tile::BiomeType b_type)
{
	groundType = g_type;
	eventType = e_type;
	biomeType = b_type;
}

float World::Tile::PerlinNoise(float x_in, float y_in, std::vector<int> p, int nOctaves)
{
	size_t x = static_cast<int>(x_in) & 255;
	size_t y = static_cast<int>(y_in) & 255;
	float xf = x_in - static_cast<int>(x_in);
	float yf = y_in - static_cast<int>(y_in);

	Maths::FVec2D topRight = Maths::FVec2D(xf - 1.0f, yf - 1.0f);
	Maths::FVec2D topLeft = Maths::FVec2D(xf, yf - 1.0f);
	Maths::FVec2D bottomRight = Maths::FVec2D(xf - 1.0f, yf);
	Maths::FVec2D bottomLeft = Maths::FVec2D(xf, yf);

	int valueTopRight = p.at(p.at(x + 1) + y + 1);
	int valueTopLeft = p.at(p.at(x) + y + 1);
	int valueBottomRight = p.at(p.at(x + 1) + y);
	int valueBottomLeft = p.at(p.at(x) + y);

	float dotTopRight = topRight.DotProduct(GetConstantVector(valueTopRight));
	float dotTopLeft = topLeft.DotProduct(GetConstantVector(valueTopLeft));
	float dotBottomRight = bottomRight.DotProduct(GetConstantVector(valueBottomRight));
	float dotBottomLeft = bottomLeft.DotProduct(GetConstantVector(valueBottomLeft));

	float u = Fade(xf);
	float v = Fade(yf);

	float output = Lerp(Lerp(dotBottomLeft, dotTopLeft, v),
		Lerp(dotBottomRight, dotTopRight, v),
		u);

	if (nOctaves > 0) return Lerp(output, PerlinNoise(x_in * 0.1f, y_in * 0.1f, p, nOctaves - 1), 0.5f);

	return output;
}

Maths::FVec2D World::Tile::GetConstantVector(int value)
{
	int h = value & 3;
	switch (h) {
	case 0:
		return Maths::FVec2D(1.0f, 1.0f);
	case 1:
		return Maths::FVec2D(-1.0f, 1.0f);
	case 2:
		return Maths::FVec2D(-1.0f, -1.0f);
	default:
		return Maths::FVec2D(1.0f, -1.0f);
	}
}

float World::Tile::Fade(float t)
{
	return ((6*t - 15)*t + 10)*t*t*t;
}

float World::Tile::Lerp(float val0, float val1, float alpha)
{
	return val0 + (val1 - val0) * alpha;
}

uint32_t World::Lehmer32(uint32_t nLehmer) const
{
	nLehmer += 0xe120fc15;
	uint64_t tmp;
	tmp = static_cast<uint64_t>(nLehmer * static_cast<uint64_t>(0x4a39b70d));
	uint32_t m1 = static_cast<uint32_t>((tmp >> 32) ^ tmp);
	tmp = static_cast<uint64_t>(m1 * static_cast<uint64_t>(0x12fad5c9));
	uint32_t m2 = static_cast<uint32_t>((tmp >> 32) ^ tmp);
	return m2;
}

int World::rndInt(int min, int max, uint32_t nLehmer) const
{
	return (Lehmer32(nLehmer) % (max - min)) + min;
}

World::World()
	:
	pGfx(GraphicsEngine::Graphics::GetInstance()),
	pKbd(CoreSystem::Keyboard::GetInstance()),
	pTimer(CoreSystem::Timer::GetInstance()),
	pPlayer(Player::GetInstance(Maths::IRect(384, 267, 32, 44), "json/player.json")),
	pGuest(std::make_unique<NPC>(Maths::IRect(-1000, -1000, 32, 44), "json/npc.json")),
	currentPlayerXPos(400),
	currentPlayerYPos(300),
	tileSpriteForest("Images/tileSheetForest.png"),
	tileSpriteDesert("Images/tileSheetDesert.png"),
	tileSpriteToundra("Images/tileSheetToundra.png"),
	tempestSprite("Images/tempest.png"),
	tempestForest(Maths::IRect(0, 0, 32, 32), 13, tempestSprite, 0.01f),
	tempestDesert(Maths::IRect(0, 32, 32, 32), 13, tempestSprite, 0.05f),
	tempestToundra(Maths::IRect(0, 64, 32, 32), 13, tempestSprite, 0.15f)
{
	//461 168 601 842 738 790

	//Init generation seed
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(0, 40);
	generationSeed = dist(rng);

	//Init weather timer
	std::uniform_real_distribution<float> distTime(40.0f, 100.0f);
	weatherTimer.ResetTimer(distTime(rng));

	//Init permutation array for perlin noise
	MakePermutation(); 

	//Init item list
	JSONParser::Reader jsonReader;
	jsonReader.ReadFile("json/itemList.json");

	auto& consumables = jsonReader.GetValueOf("Consumables");
	for (auto itr = consumables.MemberBegin(); itr != consumables.MemberEnd(); ++itr) {
		itemList.emplace_back(std::make_shared<Consumable>(itr->name.GetString(), itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt()));
	}

	auto& balls = jsonReader.GetValueOf("Balls");
	for (auto itr = balls.MemberBegin(); itr != balls.MemberEnd(); ++itr) {
		itemList.emplace_back(std::make_shared<Ball>(itr->name.GetString(), itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt()));
	}
}

void World::GenerateGrid()
{
	bTilesAreWriting = false;
	//New version : updates only if you see a new tile
	for (int i = -4; i < gridHeight + 4; i++) {
		for (int j = -4; j < gridWidth + 4; j++) {
			Maths::LLVec2D pos = Maths::LLVec2D(j + static_cast<long long>(xOffset / tileWidth), i + static_cast<long long>(yOffset / tileHeight));
			auto itr = tiles.find(pos);
			if (itr == tiles.end()) {
				bTilesAreWriting = true;
				Tile tile = Tile(j + static_cast<int>(xOffset / tileWidth), i + static_cast<int>(yOffset / tileHeight), generationSeed, *this);
				tiles.insert(std::pair<Maths::LLVec2D, Tile>(pos, tile));
				if (tile.GetEventType() == Tile::EventType::Item) {
					items.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(pos, tile.CreateItem(itemList)));
				}
				if (tile.GetGroundType() == Tile::GroundType::House0 || tile.GetGroundType() == Tile::GroundType::House3) {
					CreateHouseAt(pos);
				}
			}
		}
	}
}

void World::GenerateNewBiomePlaces()
{
	Maths::LLVec2D currentOffset = Maths::LLVec2D(long long(xOffset / tileWidth), long long(yOffset / tileHeight));
	uint32_t nLehmer = (currentOffset.x & 0xFFFF) << generationSeed | (currentOffset.y & 0xFFFF);
	if (static_cast<int>(sqrt(currentOffset.x * currentOffset.x + currentOffset.y * currentOffset.y)) % 70 == 0 && biomePlaces.find(currentOffset) == biomePlaces.end()) {
		biomePlaces.insert(std::pair<Maths::LLVec2D, std::pair<Maths::LLVec2D, Tile::BiomeType>>(currentOffset,std::pair<Maths::LLVec2D, Tile::BiomeType>(Maths::LLVec2D(rndInt(currentOffset.x, currentOffset.x + 70, nLehmer), rndInt(currentOffset.y, currentOffset.y + 50, nLehmer)), static_cast<Tile::BiomeType>(rndInt(0, 2, nLehmer)))));
	}

}

void World::UpdateTiles()
{
	for (auto t : tilesToUpdate) {
		auto itr = tiles.find(t);
		if (itr != tiles.end()) {
			if (itr->second.GetEventType() == Tile::EventType::Item) {
				auto itemPicked = items.find(t);
				items.erase(itemPicked);
			}
			itr->second.ClearEventType();
		}
	}
}

void World::UpdateTempest()
{
	tempestForest.Update(pTimer->DeltaTime());
	tempestDesert.Update(pTimer->DeltaTime());
	tempestToundra.Update(pTimer->DeltaTime());
}

void World::MakePermutation()
{
	for (int i = 0; i < 256; i++) {
		permutationArray.push_back(i);
	}

	uint32_t nLehmer = (0 & 0xFFFF) << generationSeed | (0 & 0xFFFF);

	for (int i = 0; i < 128; i++) {
		int iSwap = rndInt(128, 255, nLehmer);
		int temp = permutationArray.at(i);
		permutationArray.at(i) = permutationArray.at(iSwap);
		permutationArray.at(iSwap) = temp;
	}

	for (int i = 0; i < 256; i++) {
		permutationArray.push_back(i);
	}
}

void World::CreateHouseAt(const Maths::LLVec2D& pos)
{
	auto itr = tiles.find(pos);
	int xDir = 1;
	int yDir = 1;

	switch (itr->second.GetGroundType()) {
	case World::Tile::GroundType::House0:
		break;
	case World::Tile::GroundType::House3:
		xDir = -1;
		break;
	case World::Tile::GroundType::House12:
		yDir = -1;
		break;
	case World::Tile::GroundType::House15:
		xDir = -1;
		yDir = -1;
		break;
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Maths::LLVec2D tilePos = Maths::LLVec2D(i * static_cast<long long>(xDir), j * static_cast<long long>(yDir)) + pos;
			auto tile = tiles.find(tilePos);
			if (tiles.find(tilePos) == tiles.end()) {
				Tile newTile;
				newTile.groundType = World::Tile::GroundType(static_cast<int>(itr->second.GetGroundType()) + j * 4 * yDir + i * xDir);
				newTile.biomeType = itr->second.biomeType;
				tiles.insert(std::pair<Maths::IVec2D, Tile>(tilePos, newTile));
			}
			else {
				tile->second.groundType = World::Tile::GroundType(static_cast<int>(itr->second.GetGroundType()) + j * 4 * yDir + i * xDir);
				tile->second.ClearEventType();
			}
		}
	}
}

bool World::TileIsObstacleAt(const Maths::LLVec2D& pos)
{
	Maths::LLVec2D adaptatedPos = Maths::LLVec2D(static_cast<long long>((xOffset + pos.x) / tileWidth), static_cast<long long>((yOffset + pos.y) / tileHeight));

	//New version
	auto itr = tiles.find(adaptatedPos);
	if (itr != tiles.end()) {
		if (pPlayer->IsOnWater()) return itr->second.GetGroundType() != World::Tile::GroundType::Water;
		return itr->second.IsObstacle();
	}
	return false;
}

bool World::NextTileIsWater() const
{
	Maths::LLVec2D nextTilePos = Maths::LLVec2D(static_cast<long long>((xOffset + 400 + static_cast<long long>(pPlayer->GetLookingDirection().x) * 18) / tileWidth), static_cast<long long>((yOffset + 300 + static_cast<long long>(pPlayer->GetLookingDirection().y) * 22) / tileHeight));
	auto itr = tiles.find(nextTilePos);
	if (itr != tiles.end()) {
		return itr->second.GetGroundType() == World::Tile::GroundType::Water;
	}
	return false;
}

bool World::NextTileHas(World::Tile::EventType e_type) const
{
	Maths::LLVec2D nextTilePos = Maths::LLVec2D(static_cast<long long>((xOffset + 400 + static_cast<long long>(pPlayer->GetLookingDirection().x) * 18) / tileWidth), static_cast<long long>((yOffset + 300 + static_cast<long long>(pPlayer->GetLookingDirection().y) * 22) / tileHeight));
	auto itr = tiles.find(nextTilePos);
	if (itr != tiles.end()) {
		return itr->second.GetEventType() == e_type;
	}
	return false;
}

void World::InitFromJSON()
{
	JSONParser::Reader jsonReader;
	jsonReader.ReadFile("json/mapCoords.json");

	//Init tiles
	auto& tileInfos = jsonReader.GetValueOf("Tiles");
	for (auto itr = tileInfos.MemberBegin(); itr != tileInfos.MemberEnd(); ++itr) {
		Tile tile;
		tile.InitFromJSON(static_cast<Tile::GroundType>(itr->value.GetArray()[2].GetInt()), static_cast<Tile::EventType>(itr->value.GetArray()[3].GetInt()), static_cast<Tile::BiomeType>(itr->value.GetArray()[4].GetInt()));
		tiles.insert(std::pair<Maths::IVec2D, Tile>(Maths::IVec2D(itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt()), tile));
	}
	//Init biomes
	auto& biomeInfos = jsonReader.GetValueOf("Biomes");
	for (auto itr = biomeInfos.MemberBegin(); itr != biomeInfos.MemberEnd(); ++itr) {
		biomePlaces.insert(std::pair<Maths::LLVec2D, std::pair<Maths::IVec2D, Tile::BiomeType>>(Maths::LLVec2D(static_cast<long long>(itr->value.GetArray()[0].GetInt()), static_cast<long long>(itr->value.GetArray()[1].GetInt())), std::pair<Maths::LLVec2D, Tile::BiomeType>(Maths::LLVec2D(static_cast<long long>(itr->value.GetArray()[2].GetInt()), static_cast<long long>(itr->value.GetArray()[3].GetInt())), static_cast<Tile::BiomeType>(itr->value.GetArray()[4].GetInt()))));
	}

	//Init consumables
	if (jsonReader.IsValueAvailable("Consumables")) {
		auto& consumables = jsonReader.GetValueOf("Consumables");
		for (auto itr = consumables.MemberBegin(); itr != consumables.MemberEnd(); ++itr) {
			items.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(Maths::IVec2D(itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt()), std::make_shared<Consumable>(itr->name.GetString(), itr->value.GetArray()[2].GetInt(), itr->value.GetArray()[3].GetInt())));
		}
	}

	//Init balls
	if (jsonReader.IsValueAvailable("Balls")) {
		auto& balls = jsonReader.GetValueOf("Balls");
		for (auto itr = balls.MemberBegin(); itr != balls.MemberEnd(); ++itr) {
			items.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(Maths::IVec2D(itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt()), std::make_shared<Ball>(itr->name.GetString(), itr->value.GetArray()[2].GetInt(), itr->value.GetArray()[3].GetInt())));
		}
	}
	//Init Player
	auto& value = jsonReader.GetValueOf("Player");
	xOffset = value.GetArray()[0].GetInt();
	yOffset = value.GetArray()[1].GetInt();

	//Init seed
	generationSeed = jsonReader.GetValueOf("Seed").GetInt();
}

void World::SaveToJSON()
{
	JSONParser::Writer jsonWriter;
	//Save generation seed
	jsonWriter.AddValueForMember("Seed", generationSeed);
	//Save player position
	jsonWriter.AddValueForMember("Player", xOffset, yOffset);
	//Save tiles
	int i = 0;
	for (auto& entry : tiles) {
		jsonWriter.AddObjectMember("Tiles", "tile" + std::to_string(i), entry.first.x, entry.first.y, static_cast<int>(entry.second.GetGroundType()), static_cast<int>(entry.second.GetEventType()), static_cast<int>(entry.second.GetBiomeType()));
		i++;
	}
	//Save biomes sites
	i = 0;
	for (auto& entry : biomePlaces) {
		jsonWriter.AddObjectMember("Biomes", "biome" + std::to_string(i), entry.first.x, entry.first.y, entry.second.first.x, entry.second.first.y, static_cast<int>(entry.second.second));
		i++;
	}

	//Save items
	for (auto& entry : items) {
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
}

int World::GetNeighbourGroundType(const Maths::LLVec2D& pos, World::Tile::GroundType g_type) const
{
	int ground = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			Maths::LLVec2D neighbourPos = Maths::LLVec2D(pos.x + i, pos.y + j);
			if (neighbourPos != pos) {
				auto itr = tiles.find(neighbourPos);
				if (itr != tiles.end() && itr->second.GetGroundType() == g_type) {
					ground++;
				}
			}
		}
	}
	return ground;
}

Maths::IVec2D World::GetPlayerDirection() const
{
	return playerDirection;
}

Maths::LLVec2D World::GetPlayerPosition() const
{
	return Maths::LLVec2D(xOffset + 400, yOffset + 300);
}

void World::SetGuestPositionAndAnimation(const Maths::LLVec2D& pos, int anim, int locomotionState)
{
	pGuest->SetPosition(Maths::IVec2D(pos.x - xOffset, pos.y - yOffset), locomotionState);
	pGuest->SetAnimation(anim);
}

Uint8 World::GetWorldSeed() const
{
	return generationSeed;
}

void World::SetWorldSeed(Uint8 seed)
{
	generationSeed = seed;
}

Maths::LLVec2D World::GetLastTileToUpdate()
{
	if (lastTileToUpdate != Maths::LLVec2D(0, 0)) {
		Maths::LLVec2D output = lastTileToUpdate;
		lastTileToUpdate = Maths::LLVec2D(0, 0);
		return output;
	}
	return Maths::LLVec2D();
}

void World::AddTilesToUpdate(const Maths::LLVec2D& pos)
{
	tilesToUpdate.push_back(pos);
}

void World::Update(float speed)
{
	GenerateNewBiomePlaces();
	GenerateGrid();
	UpdateTiles();
	UpdateTempest();

	//Update Guest
	pGuest->Update(pTimer->DeltaTime());

	//Weather update
	weatherTimer.Update();
	if (weatherTimer.IsTimerDown()) {
		bTempestOn = !bTempestOn;
		std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<float> distTimer(40.0f, 100.0f);
		weatherTimer.ResetTimer(distTimer(rng));
	}

	playerDirection = Maths::IVec2D(0, 0);

	if (pKbd->KeyIsPressed(SDL_SCANCODE_UP)) {
		if (!TileIsObstacleAt(Maths::LLVec2D(400, 282))) {
			yOffset -= static_cast<int>(2 * static_cast<double>(speed));
			if (pGuest != nullptr) {
				pGuest->mRect.rect.y += static_cast<int>(2 * static_cast<double>(speed));
				pGuest->mReflectionRect.rect.y += static_cast<int>(2 * static_cast<double>(speed));
			}
			playerDirection.y--;
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_RIGHT)) {
		if (!TileIsObstacleAt(Maths::LLVec2D(418, 300))) {
			xOffset += static_cast<int>(2 * static_cast<double>(speed));
			if (pGuest != nullptr) {
				pGuest->mRect.rect.x -= static_cast<int>(2 * static_cast<double>(speed));
				pGuest->mReflectionRect.rect.x -= static_cast<int>(2 * static_cast<double>(speed));
			}
			playerDirection.x++;
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_DOWN)) {
		if (!TileIsObstacleAt(Maths::LLVec2D(400, 318))) {
			yOffset += static_cast<int>(2 * static_cast<double>(speed));
			if (pGuest != nullptr) {
				pGuest->mRect.rect.y -= static_cast<int>(2 * static_cast<double>(speed));
				pGuest->mReflectionRect.rect.y -= static_cast<int>(2 * static_cast<double>(speed));
			}
			playerDirection.y++;
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_LEFT)) {
		if (!TileIsObstacleAt(Maths::LLVec2D(382, 300))) {
			xOffset -= static_cast<int>(2 * static_cast<double>(speed));
			if (pGuest != nullptr) {
				pGuest->mRect.rect.x += static_cast<int>(2 * static_cast<double>(speed));
				pGuest->mReflectionRect.rect.x += static_cast<int>(2 * static_cast<double>(speed));
			}
			playerDirection.x--;
		}
	}
	currentPlayerXPos = xOffset + 400;
	currentPlayerYPos = yOffset + 300;


	if (pKbd->KeyIsPressed(SDL_SCANCODE_LCTRL)) {
		Maths::LLVec2D lookingAtPos = Maths::LLVec2D(xOffset + 400, yOffset + 300);
		lookingAtPos += static_cast<Maths::LLVec2D>(pPlayer->GetLookingDirection() * 18);
		lookingAtPos.x /= tileWidth;
		lookingAtPos.y /= tileHeight;

		auto itr = tiles.find(lookingAtPos);
		if (itr != tiles.end() && itr->second.GetEventType() == Tile::EventType::Item) {
			auto itemToPick = items.find(lookingAtPos);
			if (itemToPick != items.end()) {
				pPlayer->TEST_PickUpItem(itemToPick->second);
				items.erase(itemToPick);
				itr->second.ClearEventType();
			}
			lastTileToUpdate = lookingAtPos;
		}
	}
	if (pPlayer->IsUsingSpecial()) {
		switch (pPlayer->GetUsedPokemonType())
		{
		case Pokemon::Type::Fire:
			if (NextTileHas(World::Tile::EventType::Boulder)) {
				Maths::LLVec2D lookingAtPos = Maths::LLVec2D(xOffset + 400, yOffset + 300);
				lookingAtPos += static_cast<Maths::LLVec2D>(pPlayer->GetLookingDirection() * 18);
				lookingAtPos.x /= tileWidth;
				lookingAtPos.y /= tileHeight;
				auto itr = tiles.find(lookingAtPos);
				itr->second.ClearEventType();
				pPlayer->OnUseSuccess(true);
				lastTileToUpdate = lookingAtPos;
			}
			else {
				pPlayer->OnUseSuccess(false);
			}
			break;
		case Pokemon::Type::Water:
			if (NextTileIsWater()) {
				if (pPlayer->IsOnWater()) {
					pPlayer->OnUseSuccess(false);
				}
				else {
					pPlayer->OnUseSuccess(true);
					xOffset += static_cast<long long>(pPlayer->GetLookingDirection().x) * 18;
					yOffset += static_cast<long long>(pPlayer->GetLookingDirection().y) * 22;
					pPlayer->OnWater(true);
				}
			}
			else {
				if (pPlayer->IsOnWater()) {
					pPlayer->OnUseSuccess(true);
					xOffset += static_cast<long long>(pPlayer->GetLookingDirection().x) * 18;
					yOffset += static_cast<long long>(pPlayer->GetLookingDirection().y) * 22;
					pPlayer->OnWater(false);
				}
				else {
					pPlayer->OnUseSuccess(false);
				}
			}
			break;
		case Pokemon::Type::Grass:
			if (NextTileHas(World::Tile::EventType::Tree)) {
				Maths::LLVec2D lookingAtPos = Maths::LLVec2D(xOffset + 400, yOffset + 300);
				lookingAtPos += static_cast<Maths::LLVec2D>(pPlayer->GetLookingDirection() * 18);
				lookingAtPos.x /= tileWidth;
				lookingAtPos.y /= tileHeight;
				auto itr = tiles.find(lookingAtPos);
				itr->second.ClearEventType();
				pPlayer->OnUseSuccess(true);
				lastTileToUpdate = lookingAtPos;
			}
			else {
				pPlayer->OnUseSuccess(false);
			}
			break;
		default:
			break;
		}
	}
}

void World::Draw()
{
	//Draw Tiles (new version)
	for (int i = -1; i <= gridHeight; i++) {
		for (int j = -1; j <= gridWidth; j++) {
			Maths::LLVec2D pos = Maths::LLVec2D(j + static_cast<long long>(xOffset / tileWidth), i + static_cast<long long>(yOffset / tileHeight));
			auto itr = tiles.find(pos);
			if (itr != tiles.end()) {
				Maths::IRect srcRect;
				Maths::IRect destRect = Maths::IRect(static_cast<int>(itr->first.x * tileWidth - xOffset), static_cast<int>(itr->first.y * tileHeight - yOffset), tileWidth, tileHeight);
				GraphicsEngine::Sprite tileSprite;
				int layer = 0;

				switch (itr->second.GetBiomeType()) {
				case World::Tile::BiomeType::Forest:
					if (bTempestOn) tempestForest.Draw(destRect, 7);
					tileSprite = tileSpriteForest;
					break;
				case World::Tile::BiomeType::Desert:
					if (bTempestOn) tempestDesert.Draw(destRect, 7);
					tileSprite = tileSpriteDesert;
					break;
				case World::Tile::BiomeType::Toundra:
					if (bTempestOn) tempestToundra.Draw(destRect, 7);
					tileSprite = tileSpriteToundra;
					break;
				default:
					break;
				}

				//SrcRect is calculated from the value of the tile. Each tile sheet is made out of rows of 4 tiles of size 16x16. The x value is given from the modulo, the y value is given from the division 
				srcRect = Maths::IRect(16 * (static_cast<int>(itr->second.GetGroundType()) % 4), 16 * (static_cast<int>(itr->second.GetGroundType()) / 4), 16, 16);
				
				//Specific draw operation for some tiles
				switch (itr->second.GetGroundType()) {
				case World::Tile::GroundType::Water:
					layer = -5;
					break;
				default:
					break;
				}

				pGfx->DrawSprite(destRect, srcRect, tileSprite, layer);

				layer = 1;

				switch (itr->second.GetEventType()) {
				case World::Tile::EventType::Item:
					pGfx->DrawSprite(destRect, Maths::IRect(16, 80, 16, 16), tileSprite, layer);
					break;
				case World::Tile::EventType::Boulder:
					pGfx->DrawSprite(destRect, Maths::IRect(32, 80, 16, 16), tileSprite, layer);
					break;
				case World::Tile::EventType::Tree:
					pGfx->DrawSprite(destRect, Maths::IRect(48, 80, 16, 16), tileSprite, layer);
					break;
				default:
					break;
				}
			}
		}
	}

	//Draw guest (test)
	pGuest->Draw();
	pGuest->DrawReflection();
}

void World::BlendSpriteTo(GraphicsEngine::Color c)
{
	tileSpriteForest.BlendColor(c);
	tileSpriteDesert.BlendColor(c);
	tileSpriteToundra.BlendColor(c);
	tempestForest.BlendColorTo(c);
	tempestDesert.BlendColorTo(c);
	tempestToundra.BlendColorTo(c);
}

bool World::PlayerTriggersFight()
{
	return false;
	//New version
	Maths::LLVec2D currentPlayerPos = Maths::LLVec2D(static_cast<long long>(currentPlayerXPos / tileWidth), static_cast<long long>(currentPlayerYPos / tileHeight));
	Maths::LLVec2D lastPlayerPos = Maths::LLVec2D(static_cast<long long>(lastPlayerXPos / tileWidth), static_cast<long long>(lastPlayerYPos / tileHeight));
	lastPlayerXPos = currentPlayerXPos;
	lastPlayerYPos = currentPlayerYPos;
	if (currentPlayerPos != lastPlayerPos) {
		auto itr = tiles.find(currentPlayerPos);
		if (itr != tiles.end()) {
			return itr->second.PlayerTriggersFight(*this);
		}
	}
}

bool World::GoInside() const
{
	Maths::LLVec2D lookingAtPos = Maths::LLVec2D(xOffset + 400, yOffset + 300);
	lookingAtPos += static_cast<Maths::LLVec2D>(pPlayer->GetLookingDirection() * 18);
	lookingAtPos.x /= tileWidth;
	lookingAtPos.y /= tileHeight;

	auto itr = tiles.find(lookingAtPos);
	if (itr != tiles.end() && itr->second.GetGroundType() == Tile::GroundType::House13) {
		return pKbd->KeyIsPressed(SDL_SCANCODE_UP);
	}
	return false;
}
