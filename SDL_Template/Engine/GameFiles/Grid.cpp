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
	generationSeed(std::random_device{}()),
	lastPlayerXPos(xOffset + 400),
	lastPlayerYPos(yOffset + 300)
{
	tileSprite.InitSurface("Images/tileSheet.png");
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
	if (pKbd->KeyIsPressed(SDL_SCANCODE_UP)) {
		if (!NextTileIsRocks(Maths::IVec2D(400, 282))) {
			yOffset -= 4;
			currentPlayerYPos = lastPlayerYPos - 4;
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_RIGHT)) {
		if (!NextTileIsRocks(Maths::IVec2D(418, 300))) {
			xOffset += 4;
			currentPlayerXPos = lastPlayerXPos + 4;
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_DOWN)) {
		if (!NextTileIsRocks(Maths::IVec2D(400, 318))) {
			yOffset += 4;
			currentPlayerYPos = lastPlayerYPos + 4;
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_LEFT)) {
		if (!NextTileIsRocks(Maths::IVec2D(382, 300))) {
			xOffset -= 4;
			currentPlayerXPos = lastPlayerXPos - 4;
		}
	}
}

void Grid::Draw()
{
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
