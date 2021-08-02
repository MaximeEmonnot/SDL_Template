#include "House.h"

House::House()
	:
	pGfx(GraphicsEngine::Graphics::GetInstance()),
	pKbd(CoreSystem::Keyboard::GetInstance())
{
	//Sprite initialisation
	sprite.InitSurface("Images/houseTileSheet.png");

	//Init Tiles

	JSONParser::Reader jsonReader;

	jsonReader.ReadFile("json/houseTiles.json");

	auto& tileInfo = jsonReader.GetValueOf("Tiles");
	for (auto itr = tileInfo.MemberBegin(); itr != tileInfo.MemberEnd(); ++itr) {
		tiles.insert(std::pair<Maths::IVec2D, TileTypes>(Maths::IVec2D(itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt()), TileTypes(itr->value.GetArray()[2].GetInt())));
	}
	printf("flowerpot value : %d", House::TileTypes::Flower);
}

void House::Update()
{
	if (pKbd->KeyIsPressed(SDL_SCANCODE_UP)) {
		if (!IsObstacle(Maths::IVec2D(400, 282)))
		{
			yOffset -= 2;
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_RIGHT)) {
		if (!IsObstacle(Maths::IVec2D(418, 300)) && !IsChair(Maths::IVec2D(418, 300))) {
			xOffset += 2;
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_DOWN)) {
		if (!IsObstacle(Maths::IVec2D(400, 318))) {
			yOffset += 2;
		}
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_LEFT)) {
		if (!IsObstacle(Maths::IVec2D(382, 300)) && !IsChair(Maths::IVec2D(382, 300))) {
			xOffset -= 2;
		}
	}
}

void House::Draw()
{
	for (int i = -1; i < gridWidth; i++) {
		for (int j = -1; j < gridHeight; j++) {
			Maths::IVec2D pos = Maths::IVec2D(i + int((xOffset - 400) / tileWidth), j + int((yOffset - 300) / tileHeight));
			auto itr = tiles.find(pos);
			if (itr != tiles.end()) {
				int tileType = itr->second & (~House::TileTypes::Flower);
				tileType &= (~House::TileTypes::FlowerPot);
				Maths::IRect srcRect = Maths::IRect((tileType % 6) * 16, (int(tileType / 6)) * 16, 16, 16);
				pGfx->DrawSprite(Maths::IRect(itr->first.x * tileWidth - xOffset + 400, itr->first.y * tileHeight - yOffset + 300, tileWidth, tileHeight),srcRect, sprite);
				if ((itr->second & House::TileTypes::FlowerPot) == House::TileTypes::FlowerPot) {
					pGfx->DrawSprite(Maths::IRect(itr->first.x * tileWidth - xOffset + 400, itr->first.y * tileHeight - yOffset + 300, tileWidth, tileHeight), Maths::IRect(48, 80, 16, 16), sprite);
				}
				else if ((itr->second & House::TileTypes::Flower) == House::TileTypes::Flower) {
					pGfx->DrawSprite(Maths::IRect(itr->first.x * tileWidth - xOffset + 400, itr->first.y * tileHeight - yOffset + 300, tileWidth, tileHeight), Maths::IRect(64, 80, 16, 16), sprite);
				}
			}
		}
	}
}

bool House::IsObstacle(Maths::IVec2D nextPos)
{
	Maths::IVec2D pos = Maths::IVec2D(int((nextPos.x + xOffset - 400) / tileWidth), int((nextPos.y + yOffset - 300) / tileHeight));
	auto itr = tiles.find(pos);
	if (itr != tiles.end()) {
		return (itr->second & House::TileTypes::FlowerPot) == House::TileTypes::FlowerPot ||
			(itr->second & House::TileTypes::Flower) == House::TileTypes::Flower ||
			!((itr->second == House::TileTypes::ChairL) ||
			(itr->second == House::TileTypes::ChairR) ||
			(itr->second == House::TileTypes::Carpet0) ||
			(itr->second == House::TileTypes::Carpet1) ||
			((itr->second & House::TileTypes::Floor0) == House::TileTypes::Floor0 ||
			(itr->second & House::TileTypes::Floor1) == House::TileTypes::Floor1 ||
			(itr->second & House::TileTypes::Floor2) == House::TileTypes::Floor2));
	}
	return true;
}

bool House::IsChair(Maths::IVec2D nextPos)
{
	Maths::IVec2D pos = Maths::IVec2D((nextPos.x + xOffset - 400) / tileWidth, (nextPos.y + yOffset - 300) / tileHeight);
	auto itr = tiles.find(pos);
	if (itr != tiles.end()) {
		switch (itr->second) {
		case House::TileTypes::ChairL:
		case House::TileTypes::ChairR:
			return true;
		default:
			return false;
		}
	}
	return false;
}
