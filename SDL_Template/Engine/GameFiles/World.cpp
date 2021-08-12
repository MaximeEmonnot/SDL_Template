#include "World.h"
#include <iostream>

World::Tile::Tile(int x_world_pos, int y_world_pos, int seed, const World& grid)
{
	uint32_t nLehmer = (x_world_pos & 0xFFFF) << seed | (y_world_pos & 0xFFFF);

	int probaGrass = 0;
	int probaRocks = 0;
		
	Maths::LLVec2D worldPos = Maths::LLVec2D(x_world_pos, y_world_pos);

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

	if (rndInt(0, 1000, nLehmer) < 1) {
		groundType = GroundType::House0;
	}
	else {
		if (rndInt(0, 100, nLehmer) < probaRocks) {
			groundType = GroundType::Rocks;
		}
		else if (rndInt(0, 100, nLehmer) < probaGrass) {
			groundType = GroundType::Grass;
			if (rndInt(0, 200, nLehmer) < 1) {
				eventType = EventType::Item;
			}
		}
		else {
			// 50% grass 25% Forest 15% water 8% Rocks 2% Sand
			if (rndInt(0, 10, nLehmer) < 8) {
				groundType = GroundType::Sand;
				if (rndInt(0, 100, nLehmer) < 1) {
					eventType = EventType::Item;
				}
			}
			else {
				if (rndInt(0, 4, nLehmer) < 3) {
					groundType = GroundType::Grass;
					if (rndInt(0, 250, nLehmer) < 1) {
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

std::shared_ptr<Item> World::Tile::CreateItem(std::vector<std::shared_ptr<Item>> items)
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

World::Tile::GroundType World::Tile::GetGroundType() const
{
	return groundType;
}

bool World::Tile::IsObstacle() const
{
	switch (groundType)
	{
	case World::Tile::GroundType::Rocks:
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
	return eventType == EventType::Item;
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

void World::Tile::InitFromJSON(Tile::GroundType g_type, Tile::EventType e_type)
{
	groundType = g_type;
	eventType = e_type;
}

float World::Tile::PerlinNoise(float x_in, float y_in)
{
	int x = int(x_in) & 255;
	int y = int(y_in) & 255;
	float xf = x_in - int(x_in);
	float yf = y_in - int(y_in);

	Maths::FVec2D topRight = Maths::FVec2D(xf - 1.0f, yf - 1.0f);
	Maths::FVec2D topLeft = Maths::FVec2D(xf, yf - 1.0f);
	Maths::FVec2D bottomRight = Maths::FVec2D(xf - 1.0f, yf);
	Maths::FVec2D bottomLeft = Maths::FVec2D(xf, yf);

	std::vector<int> P;
	for (size_t i = 0; i < 256; i++) {
		P.push_back(i);
	}




	return 0.0f;
}

uint32_t World::Tile::Lehmer32(uint32_t nLehmer)
{
	nLehmer += 0xe120fc15;
	uint64_t tmp;
	tmp = (uint64_t)nLehmer * 0x4a39b70d;
	uint32_t m1 = (tmp >> 32) ^ tmp;
	tmp = (uint64_t)m1 * 0x12fad5c9;
	uint32_t m2 = (tmp >> 32) ^ tmp;
	return m2;
}

int World::Tile::rndInt(int min, int max, uint32_t nLehmer)
{
	return (Lehmer32(nLehmer) % (max - min)) + min;
}

World::World()
	:
	pGfx(GraphicsEngine::Graphics::GetInstance()),
	pKbd(CoreSystem::Keyboard::GetInstance()),
	pPlayer(Player::GetInstance(Maths::IRect(384, 267, 32, 44), "json/player.json")),
	currentPlayerXPos(400),
	currentPlayerYPos(300),
	tileSprite("Images/tileSheet.png")
{
	//461 168 601 842 738 790

	//Init generation seed
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(-20, 20);
	generationSeed = dist(rng);

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
	//New version : updates only if you see a new tile
	for (int i = -1; i <= gridHeight; i++) {
		for (int j = -1; j <= gridWidth; j++) {
			Maths::LLVec2D pos = Maths::LLVec2D(j + int(xOffset / tileWidth), i + int(yOffset / tileHeight));
			auto itr = tiles.find(pos);
			if (itr == tiles.end()) {
				Tile tile = Tile(j + int(xOffset / tileWidth), i + int(yOffset / tileHeight), generationSeed, *this);
				tiles.insert(std::pair<Maths::IVec2D, Tile>(pos, tile));
				if (tile.GetEventType() == Tile::EventType::Item) {
					items.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(pos, tile.CreateItem(itemList)));
				}
				if (tile.GetGroundType() == Tile::GroundType::House0) {
					CreateHouseAt(pos);
				}
			}
		}
	}

}

void World::CreateHouseAt(const Maths::LLVec2D& pos)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Maths::LLVec2D tilePos = Maths::LLVec2D(i, j) + pos;
			if (tilePos != pos) {
				Tile tile;
				tile.groundType = World::Tile::GroundType(int(World::Tile::GroundType::House0) + j * 4 + i);
				tiles.insert(std::pair<Maths::IVec2D, Tile>(tilePos, tile));
			}
		}
	}
}

bool World::TileIsObstacleAt(const Maths::LLVec2D& pos)
{
	Maths::LLVec2D adaptatedPos = Maths::LLVec2D(int((xOffset + pos.x) / tileWidth), int((yOffset + pos.y) / tileWidth));

	//New version
	auto itr = tiles.find(adaptatedPos);
	if (itr != tiles.end()) {
		return itr->second.IsObstacle();
	}
}

void World::InitFromJSON()
{
	JSONParser::Reader jsonReader;
	jsonReader.ReadFile("json/mapCoords.json");

	//Init tiles
	auto& tileInfos = jsonReader.GetValueOf("Tiles");
	for (auto itr = tileInfos.MemberBegin(); itr != tileInfos.MemberEnd(); ++itr) {
		Tile tile;
		tile.InitFromJSON(Tile::GroundType(itr->value.GetArray()[2].GetInt()), Tile::EventType(itr->value.GetArray()[3].GetInt()));
		tiles.insert(std::pair<Maths::IVec2D, Tile>(Maths::IVec2D(itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt()), tile));
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
		jsonWriter.AddObjectMember("Tiles", "tile" + std::to_string(i), entry.first.x, entry.first.y, int(entry.second.GetGroundType()), int(entry.second.GetEventType()));
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

void World::Update(float speed)
{
	GenerateGrid();

	if (pKbd->KeyIsPressed(SDL_SCANCODE_UP)) {
		if (!TileIsObstacleAt(Maths::LLVec2D(400, 282))) {
			yOffset -= int(2 * speed);
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_RIGHT)) {
		if (!TileIsObstacleAt(Maths::LLVec2D(418, 300))) {
			xOffset += int(2 * speed);
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_DOWN)) {
		if (!TileIsObstacleAt(Maths::LLVec2D(400, 318))) {
			yOffset += int(2 * speed);
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_LEFT)) {
		if (!TileIsObstacleAt(Maths::LLVec2D(382, 300))) {
			xOffset -= int(2 * speed);
		}
	}
	currentPlayerXPos = xOffset + 400;
	currentPlayerYPos = yOffset + 300;


	if (pKbd->KeyIsPressed(SDL_SCANCODE_LCTRL)) {
		Maths::LLVec2D lookingAtPos = Maths::LLVec2D(xOffset + 400, yOffset + 300);
		lookingAtPos += Maths::LLVec2D(pPlayer->GetLookingDirection() * 18);
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
		}
	}
}

void World::Draw()
{
	//Draw Tiles (new version)
	for (int i = -1; i <= gridHeight; i++) {
		for (int j = -1; j <= gridWidth; j++) {
			Maths::LLVec2D pos = Maths::LLVec2D(j + int(xOffset / tileWidth), i + int(yOffset / tileHeight));
			auto itr = tiles.find(pos);
			if (itr != tiles.end()) {
				Maths::IRect srcRect;
				switch (itr->second.GetGroundType()) {
				case World::Tile::GroundType::Grass:
					srcRect = Maths::IRect(0, 0, 16, 16);
					break;
				case World::Tile::GroundType::Sand:
					srcRect = Maths::IRect(16, 0, 16, 16);
					break;
				case World::Tile::GroundType::Rocks:
					srcRect = Maths::IRect(32, 0, 16, 16);
					break;
				case World::Tile::GroundType::House0:
					srcRect = Maths::IRect(0, 16, 16, 16);
					break;
				case World::Tile::GroundType::House1:
					srcRect = Maths::IRect(16, 16, 16, 16);
					break;
				case World::Tile::GroundType::House2:
					srcRect = Maths::IRect(32, 16, 16, 16);
					break;
				case World::Tile::GroundType::House3:
					srcRect = Maths::IRect(48, 16, 16, 16);
					break;
				case World::Tile::GroundType::House4:
					srcRect = Maths::IRect(0, 32, 16, 16);
					break;
				case World::Tile::GroundType::House5:
					srcRect = Maths::IRect(16, 32, 16, 16);
					break;
				case World::Tile::GroundType::House6:
					srcRect = Maths::IRect(32, 32, 16, 16);
					break;
				case World::Tile::GroundType::House7:
					srcRect = Maths::IRect(48, 32, 16, 16);
					break;
				case World::Tile::GroundType::House8:
					srcRect = Maths::IRect(0, 48, 16, 16);
					break;
				case World::Tile::GroundType::House9:
					srcRect = Maths::IRect(16, 48, 16, 16);
					break;
				case World::Tile::GroundType::House10:
					srcRect = Maths::IRect(32, 48, 16, 16);
					break;
				case World::Tile::GroundType::House11:
					srcRect = Maths::IRect(48, 48, 16, 16);
					break;
				case World::Tile::GroundType::House12:
					srcRect = Maths::IRect(0, 64, 16, 16);
					break;
				case World::Tile::GroundType::House13:
					srcRect = Maths::IRect(16, 64, 16, 16);
					break;
				case World::Tile::GroundType::House14:
					srcRect = Maths::IRect(32, 64, 16, 16);
					break;
				case World::Tile::GroundType::House15:
					srcRect = Maths::IRect(48, 64, 16, 16);
					break;
				default:
					break;
				}
				pGfx->DrawSprite(Maths::IRect(int(itr->first.x * tileWidth - xOffset), int(itr->first.y * tileHeight - yOffset), tileWidth, tileHeight), srcRect, tileSprite);

				if (itr->second.GetEventType() == Tile::EventType::Item) {
					pGfx->DrawSprite(Maths::IRect(int(itr->first.x * tileWidth - xOffset), int(itr->first.y * tileHeight - yOffset), tileWidth, tileHeight), Maths::IRect(48, 0, 16, 16), tileSprite);
				}
			}
		}
	}

}

void World::BlendSpriteTo(GraphicsEngine::Color c)
{
	tileSprite.BlendColor(c);
}

bool World::PlayerTriggersFight()
{
	//New version
	Maths::LLVec2D currentPlayerPos = Maths::LLVec2D(int(currentPlayerXPos / tileWidth), int(currentPlayerYPos / tileHeight));
	Maths::LLVec2D lastPlayerPos = Maths::LLVec2D(int(lastPlayerXPos / tileWidth), int(lastPlayerYPos / tileHeight));
	lastPlayerXPos = currentPlayerXPos;
	lastPlayerYPos = currentPlayerYPos;
	if (currentPlayerPos != lastPlayerPos) {
		auto itr = tiles.find(currentPlayerPos);
		if (itr != tiles.end()) {
			return itr->second.PlayerTriggersFight(*this);
		}
	}
	return false;
}

bool World::GoInside() const
{
	Maths::LLVec2D lookingAtPos = Maths::LLVec2D(xOffset + 400, yOffset + 300);
	lookingAtPos += Maths::LLVec2D(pPlayer->GetLookingDirection() * 18);
	lookingAtPos.x /= tileWidth;
	lookingAtPos.y /= tileHeight;

	auto itr = tiles.find(lookingAtPos);
	if (itr != tiles.end() && itr->second.GetGroundType() == Tile::GroundType::House13) {
		return pKbd->KeyIsPressed(SDL_SCANCODE_UP);
	}
	return false;
}
