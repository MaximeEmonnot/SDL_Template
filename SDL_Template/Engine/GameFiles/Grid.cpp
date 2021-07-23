#include "Grid.h"
#include <random>

Grid::Tile::Tile(int x_world_pos, int y_world_pos, int seed)
	:
	worldPos(x_world_pos, y_world_pos)
{
	nLehmer = (x_world_pos & 0xFFFF) << seed | (y_world_pos & 0xFFFF);

	// 50% grass 25% Forest 15% water 8% Rocks 2% Sand
	if (rndInt(0, 10) < 8) {
		type = Type::Sand;
	}
	else {
		if (rndInt(0, 4) < 3) {
			type = Type::Grass;
		}
		else {
			type = Type::Rocks;
		}
	}
}

Grid::Tile::Type Grid::Tile::GetType()
{
	return type;
}

Maths::IVec2D Grid::Tile::GetWorldPosition()
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
	pPlayer(Player::GetInstance(Maths::IRect(384, 284, 32, 32), "json/kirby.json")),
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
	tiles.clear();
	for (int i = -1; i <= gridHeight; i++) {
		for (int j = -1; j <= gridWidth; j++) {
			tiles.emplace_back(j + int (xOffset / tileWidth), i + int (yOffset / tileHeight), 12);
		}
	}
}

void Grid::GenerateItems()
{
	int k = 0;
	for (size_t i = 0; i < 1000; i++) {
		for (size_t j = 0; j < 1000; j++) {
			uint32_t nLehmer = (j & 0xFFFF) << 12 | (i & 0xFFFF);
			nLehmer += 0xe120fc15;
			uint64_t tmp;
			tmp = (uint64_t)nLehmer * 0x4a39b70d;
			uint32_t m1 = (tmp >> 32) ^ tmp;
			tmp = (uint64_t)m1 * 0x12fad5c9;
			uint32_t m2 = (tmp >> 32) ^ tmp;
			if (m2 % 5 < 4) {
				if (m2 % 250 < 1) {
					std::mt19937 rng(std::random_device{}());
					std::uniform_int_distribution<int> dist(0, 100);
					if (dist(rng) < 40) {
						//Consumables
						if (dist(rng) < 70){
						// Potion 35%
						items.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(Maths::IVec2D(i, j), itemList.at(0)));
						}
						else if (dist(rng) < 55) {
						// Super potion 20%
						items.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(Maths::IVec2D(i, j), itemList.at(1)));
						}
						else if (dist(rng) < 70) {
						// Hyper potion 15%
						items.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(Maths::IVec2D(i, j), itemList.at(2)));
						}
						else if (dist(rng) < 73) {
						// Potion max 3%
						items.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(Maths::IVec2D(i, j), itemList.at(3)));
						}
						else if (dist(rng) < 83) {
						// Attack+ 10%
						items.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(Maths::IVec2D(i, j), itemList.at(4)));
						}
						else if (dist(rng) < 93) {
						// Defense+ 10%
						items.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(Maths::IVec2D(i, j), itemList.at(5)));
						}
						else if (dist(rng) < 98) {
						// HP+ 5%
						items.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(Maths::IVec2D(i, j), itemList.at(6)));
						}
						else {
						//Super candy 2%
						items.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(Maths::IVec2D(i, j), itemList.at(7)));
						}
					}
					else {
						//Balls
						if (dist(rng) < 45) {
						//Pokeball 45%
						items.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(Maths::IVec2D(i, j), itemList.at(8)));
						}
						else if (dist(rng) < 78){
						//Superball 33%
						items.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(Maths::IVec2D(i, j), itemList.at(9)));
						}
						else if (dist(rng) < 99) {
						//HyperBall 21%
						items.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(Maths::IVec2D(i, j), itemList.at(10)));
						}
						else {
						//Masterball 1%
						items.insert(std::pair<Maths::IVec2D, std::shared_ptr<Item>>(Maths::IVec2D(i, j), itemList.at(11)));
						}
					}
					k++;
				}
			}
		}
	}
}

bool Grid::NextTileIsRocks(const Maths::IVec2D& pos)
{
	for (auto& tile : tiles) {
		if (tile.GetType() == Grid::Tile::Type::Rocks) {
			if (tile.GetWorldPosition() == Maths::IVec2D(int((xOffset + pos.x) / tileWidth), int((yOffset + pos.y) / tileWidth)))
				return true;
		}
	}
	return false;
}

void Grid::Update()
{
	GenerateGrid();
	if (items.empty()) {
		GenerateItems();
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_UP)) {
		if (!NextTileIsRocks(Maths::IVec2D(400, 282)) &&
			items.find(Maths::IVec2D( (xOffset + 400) / tileWidth, (yOffset + 282) / tileHeight)) == items.end()) {
			yOffset -= 4;
			currentPlayerYPos = lastPlayerYPos - 4;
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_RIGHT)) {
		if (!NextTileIsRocks(Maths::IVec2D(418, 300)) &&
			items.find(Maths::IVec2D((xOffset + 418) / tileWidth, (yOffset + 300) / tileHeight)) == items.end()) {
			xOffset += 4;
			currentPlayerXPos = lastPlayerXPos + 4;
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_DOWN)) {
		if (!NextTileIsRocks(Maths::IVec2D(400, 318)) &&
			items.find(Maths::IVec2D((xOffset + 400) / tileWidth, (yOffset + 318) / tileHeight)) == items.end()) {
			yOffset += 4;
			currentPlayerYPos = lastPlayerYPos + 4;
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_LEFT)) {
		if (!NextTileIsRocks(Maths::IVec2D(382, 300)) &&
			items.find(Maths::IVec2D((xOffset + 382) / tileWidth, (yOffset + 300) / tileHeight)) == items.end()) {
			xOffset -= 4;
			currentPlayerXPos = lastPlayerXPos - 4;
		}
	}

	if (pKbd->KeyIsPressed(SDL_SCANCODE_LCTRL)) {
		auto upTile = items.find(Maths::IVec2D((xOffset + 400) / tileWidth, (yOffset + 282) / tileHeight));
		auto rightTile = items.find(Maths::IVec2D((xOffset + 418) / tileWidth, (yOffset + 300) / tileHeight));
		auto downTile = items.find(Maths::IVec2D((xOffset + 400) / tileWidth, (yOffset + 318) / tileHeight));
		auto leftTile = items.find(Maths::IVec2D((xOffset + 382) / tileWidth, (yOffset + 300) / tileHeight));

		if (pPlayer->GetLookingDirection() == Maths::IVec2D(0, -1) &&
			upTile != items.end()) {
			pPlayer->TEST_PickUpItem(upTile->second);
			items.erase(upTile);
		}
		else if (pPlayer->GetLookingDirection() == Maths::IVec2D(1, 0) &&
			rightTile != items.end()) {
			pPlayer->TEST_PickUpItem(rightTile->second);
			items.erase(rightTile);
		}
		else if (pPlayer->GetLookingDirection() == Maths::IVec2D(0, 1) &&
			downTile != items.end()) {
			pPlayer->TEST_PickUpItem(downTile->second);
			items.erase(downTile);
		}
		else if (pPlayer->GetLookingDirection() == Maths::IVec2D(-1, 0) &&
			leftTile != items.end()) {
			pPlayer->TEST_PickUpItem(leftTile->second);
			items.erase(leftTile);
		}
	}

}

void Grid::Draw()
{
	//Draw Tiles
	for (auto& tile : tiles) {
		Maths::IRect srcRect;
		switch (tile.GetType()) {
		case Grid::Tile::Type::Grass :
			srcRect = Maths::IRect(0, 0, 16, 16);
			break;
		case Grid::Tile::Type::Sand:
			srcRect = Maths::IRect(16, 0, 16, 16);
			break;
		case Grid::Tile::Type::Rocks:
			srcRect = Maths::IRect(32, 0, 16, 16);
			break;
		default:
			break;
		}
		pGfx->DrawSprite(Maths::IRect(tile.GetWorldPosition().x * tileWidth - xOffset, tile.GetWorldPosition().y * tileHeight - yOffset, tileWidth, tileHeight), srcRect, tileSprite);
	}

	//Draw Items
	for (auto& item : items) {
		pGfx->DrawSprite(Maths::IRect(item.first.x * tileWidth - xOffset, item.first.y * tileHeight - yOffset, tileWidth, tileHeight), Maths::IRect(48, 0, 16, 16), tileSprite);
	}
}

bool Grid::PlayerTriggersFight()
{
	if (Maths::IVec2D(int(currentPlayerXPos / tileWidth), int(currentPlayerYPos / tileHeight)) != Maths::IVec2D(int(lastPlayerXPos / tileWidth), int(lastPlayerYPos / tileHeight))) {
		for (auto& tile : tiles) {
			if (tile.GetType() == Grid::Tile::Type::Grass) {
				if (tile.PlayerTriggersFight(int((xOffset + 400) / tileWidth), int((yOffset + 300) / tileHeight)))
					return true;
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
