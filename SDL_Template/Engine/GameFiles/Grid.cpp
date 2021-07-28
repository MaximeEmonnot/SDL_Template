#include "Grid.h"
#include <random>

Grid::Tile::Tile(int x_world_pos, int y_world_pos, int seed)
	:
	worldPos(x_world_pos, y_world_pos)
{
	nLehmer = (x_world_pos & 0xFFFF) << seed | (y_world_pos & 0xFFFF);

	// 50% grass 25% Forest 15% water 8% Rocks 2% Sand
	if (rndInt(0, 10) < 8) {
		groundType = GroundType::Sand;
		if (rndInt(0, 100) < 1) {
			eventType = EventType::Item;
		}
	}
	else {
		if (rndInt(0, 4) < 3) {
			groundType = GroundType::Grass;
			if (rndInt(0, 250) < 1) {
				eventType = EventType::Item;
			}
		}
		else {
			groundType = GroundType::Rocks;
		}
	}
}

std::shared_ptr<Item> Grid::Tile::CreateItem(std::vector<std::shared_ptr<Item>> items)
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

void Grid::Tile::ClearEventType()
{
	eventType = Grid::Tile::EventType::None;
}

Grid::Tile::EventType Grid::Tile::GetEventType() const
{
	return eventType;
}

Grid::Tile::GroundType Grid::Tile::GetGroundType() const
{
	return groundType;
}

Maths::IVec2D Grid::Tile::GetWorldPosition() const
{
	return worldPos;
}

bool Grid::Tile::PlayerTriggersFight(int player_x_pos, int player_y_pos)
{
	if (worldPos == Maths::IVec2D(player_x_pos, player_y_pos)) {
		std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<int> dist(0, 20);
		if (dist(rng) == 1)
			return true;
	}
	return false;
}

void Grid::Tile::InitFromJSON(int x_pos, int y_pos, Tile::GroundType g_type, Tile::EventType e_type)
{
	worldPos = Maths::IVec2D(x_pos, y_pos);
	groundType = g_type;
	eventType = e_type;
}

uint32_t Grid::Tile::Lehmer32()
{
	nLehmer += 0xe120fc15;
	uint64_t tmp;
	tmp = (uint64_t)nLehmer * 0x4a39b70d;
	uint32_t m1 = (tmp >> 32) ^ tmp;
	tmp = (uint64_t)m1 * 0x12fad5c9;
	uint32_t m2 = (tmp >> 32) ^ tmp;
	return m2;
}

int Grid::Tile::rndInt(int min, int max)
{
	return (Lehmer32() % (max - min)) + min;
}

Grid::Grid()
	:
	pGfx(GraphicsEngine::Graphics::GetInstance()),
	pKbd(CoreSystem::Keyboard::GetInstance()),
	pPlayer(Player::GetInstance(Maths::IRect(384, 267, 32, 44), "json/player.json")),
	generationSeed(std::random_device{}()),
	lastPlayerXPos(xOffset + 400),
	lastPlayerYPos(yOffset + 300)
{
	tileSprite.InitSurface("Images/tileSheet.png");

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

void Grid::GenerateGrid()
{
	//New version : updates only if you see a new tile
	for (int i = -1; i <= gridHeight; i++) {
		for (int j = -1; j <= gridWidth; j++) {
			Maths::IVec2D pos = Maths::IVec2D(j + int(xOffset / tileWidth), i + int(yOffset / tileHeight));
			auto itr = tiles.find(pos);
			if (itr == tiles.end()) {
				Tile tile = Tile(j + int(xOffset / tileWidth), i + int(yOffset / tileHeight), 12);
				tiles.insert(std::pair<Maths::IVec2D, Tile>(pos, tile));
				if (tile.GetEventType() == Tile::EventType::Item) {
					items.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(pos, tile.CreateItem(itemList)));
				}
			}
		}
	}

}

bool Grid::TileIsObstacleAt(const Maths::IVec2D& pos)
{
	Maths::IVec2D adaptatedPos = Maths::IVec2D(int((xOffset + pos.x) / tileWidth), int((yOffset + pos.y) / tileWidth));

	//New version
	auto itr = tiles.find(adaptatedPos);
	if (itr != tiles.end()) {
		return (itr->second.GetGroundType() == Grid::Tile::GroundType::Rocks) || (itr->second.GetEventType() == Grid::Tile::EventType::Item);
	}
}

void Grid::InitFromJSON()
{
	JSONParser::Reader jsonReader;
	jsonReader.ReadFile("json/mapCoords.json");

	//Init tiles
	auto& tileInfos = jsonReader.GetValueOf("Tiles");
	for (auto itr = tileInfos.MemberBegin(); itr != tileInfos.MemberEnd(); ++itr) {
		Tile tile;
		tile.InitFromJSON(itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt(), Tile::GroundType(itr->value.GetArray()[2].GetInt()), Tile::EventType(itr->value.GetArray()[3].GetInt()));
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

	auto& value = jsonReader.GetValueOf("Player");
	xOffset = value.GetArray()[0].GetInt(); 
	yOffset = value.GetArray()[1].GetInt();
}

void Grid::SaveToJSON()
{
	JSONParser::Writer jsonWriter;
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

void Grid::Update()
{
	GenerateGrid();
	if (pKbd->KeyIsPressed(SDL_SCANCODE_UP)) {
		if (!TileIsObstacleAt(Maths::IVec2D(400, 282))) {
			yOffset -= 2;
			currentPlayerYPos = lastPlayerYPos - 2;
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_RIGHT)) {
		if (!TileIsObstacleAt(Maths::IVec2D(418, 300))) {
			xOffset += 2;
			currentPlayerXPos = lastPlayerXPos + 2;
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_DOWN)) {
		if (!TileIsObstacleAt(Maths::IVec2D(400, 318))) {
			yOffset += 2;
			currentPlayerYPos = lastPlayerYPos + 2;
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_LEFT)) {
		if (!TileIsObstacleAt(Maths::IVec2D(382, 300))) {
			xOffset -= 2;
			currentPlayerXPos = lastPlayerXPos - 2;
		}
	}
	
	if (pKbd->KeyIsPressed(SDL_SCANCODE_LCTRL)) {
		Maths::IVec2D lookingAtPos = Maths::IVec2D(xOffset + 400, yOffset + 300);
		lookingAtPos += pPlayer->GetLookingDirection() * 18;
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

void Grid::Draw()
{ 
	//Draw Tiles (new version)
	for (int i = -1; i <= gridHeight; i++) {
		for (int j = -1; j <= gridWidth; j++) {
			Maths::IVec2D pos = Maths::IVec2D(j + int(xOffset / tileWidth), i + int(yOffset / tileHeight));
			auto itr = tiles.find(pos);
			if (itr != tiles.end()) {
				Maths::IRect srcRect;
				switch (itr->second.GetGroundType()) {
				case Grid::Tile::GroundType::Grass:
					srcRect = Maths::IRect(0, 0, 16, 16);
					break;
				case Grid::Tile::GroundType::Sand:
					srcRect = Maths::IRect(16, 0, 16, 16);
					break;
				case Grid::Tile::GroundType::Rocks:
					srcRect = Maths::IRect(32, 0, 16, 16);
					break;
				default:
					break;
				}
				if (pTimer->IsNightTime()) tileSprite.BlendColor(GraphicsEngine::Color(64, 64, 128, 128));
				pGfx->DrawSprite(Maths::IRect(itr->first.x * tileWidth - xOffset, itr->first.y * tileHeight - yOffset, tileWidth, tileHeight), srcRect, tileSprite);
			
				if (itr->second.GetEventType() == Tile::EventType::Item) {
					pGfx->DrawSprite(Maths::IRect(itr->first.x * tileWidth - xOffset, itr->first.y * tileHeight - yOffset, tileWidth, tileHeight), Maths::IRect(48, 0, 16, 16), tileSprite);
				}
			}
		}
	}

}

bool Grid::PlayerTriggersFight()
{
	if (Maths::IVec2D(int(currentPlayerXPos / tileWidth), int(currentPlayerYPos / tileHeight)) != Maths::IVec2D(int(lastPlayerXPos / tileWidth), int(lastPlayerYPos / tileHeight))) {
		for (auto& tile : tiles) {
			if (tile.second.GetGroundType() == Grid::Tile::GroundType::Grass) {
				return tile.second.PlayerTriggersFight(int((xOffset + 400) / tileWidth), int((yOffset + 300) / tileHeight));
			}
		}
	}
	lastPlayerXPos = currentPlayerXPos;
	lastPlayerYPos = currentPlayerYPos;
	return false;
}

void Grid::SetWorldPosition(Maths::IVec2D pos)
{
	xOffset = pos.x;
	yOffset = pos.y;
}

Maths::IVec2D Grid::GetWorldPosition()
{
	return Maths::IVec2D(xOffset, yOffset);
}
