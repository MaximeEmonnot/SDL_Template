#include "House.h"

House::House()
	:
	pGfx(GraphicsEngine::Graphics::GetInstance()),
	pKbd(CoreSystem::Keyboard::GetInstance()),
	pTimer(CoreSystem::Timer::GetInstance()),
	pPlayer(Player::GetInstance(Maths::IRect(384, 267, 32, 44), "json/player.json")),
	text(Maths::IRect(Maths::IRect(25, 500, 200, 75))),
	sprite("Images/houseTileSheet.png")
{
	//Sprite initialisation
	//sprite.InitSurface("Images/houseTileSheet.png");

	//Init Tiles

	JSONParser::Reader jsonReader;

	jsonReader.ReadFile("json/houseTiles.json");

	auto& tileInfo = jsonReader.GetValueOf("Tiles");
	for (auto itr = tileInfo.MemberBegin(); itr != tileInfo.MemberEnd(); ++itr) {
		int tileValue = (itr->value.GetArray()[2].GetInt() & (~static_cast<int>(TileTypes::NPCTile)));
		tiles.insert(std::pair<Maths::IVec2D, int>(Maths::IVec2D(itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt()), tileValue));
		if ((itr->value.GetArray()[2].GetInt() & static_cast<int>(TileTypes::NPCTile)) == static_cast<int>(TileTypes::NPCTile)) {
			pNpc = std::make_unique<NPC>(Maths::IRect(itr->value.GetArray()[0].GetInt() * tileWidth - xOffset + 400, itr->value.GetArray()[1].GetInt() * tileHeight - yOffset + 282, 32, 44), Maths::IVec2D(itr->value.GetArray()[0].GetInt() * tileWidth, itr->value.GetArray()[1].GetInt() * tileHeight), "json/npc.json", std::make_unique<AIWalking>(tiles));
		}
	}
}

void House::Update(float speed)
{
	pNpc->Update(pTimer->DeltaTime());
	pNpc->UpdateAI();

	if (!pPlayer->IsTalking()) {
		if (pKbd->KeyIsPressed(SDL_SCANCODE_UP)) {
			if (!IsObstacle(Maths::IVec2D(0, -18)) &&
				!IsOccupedByNPC(Maths::IVec2D(0, -18)))
			{
				yOffset -= int(2 * speed);
				pNpc->Move(0, int(2 * speed));
			}
		}
		if (pKbd->KeyIsPressed(SDL_SCANCODE_RIGHT)) {
			if (!IsObstacle(Maths::IVec2D(18, 0)) &&
				!IsChair(Maths::IVec2D(18, 0)) &&
				!IsOccupedByNPC(Maths::IVec2D(18, 0))) {
				xOffset += int(2 * speed);
				pNpc->Move(int(-2 * speed), 0);
			}
		}
		if (pKbd->KeyIsPressed(SDL_SCANCODE_DOWN)) {
			if (!IsObstacle(Maths::IVec2D(0, 18)) &&
				!IsOccupedByNPC(Maths::IVec2D(0, 18))) {
				yOffset += int(2 * speed);
				pNpc->Move(0, int(-2 * speed));
			}
		}
		if (pKbd->KeyIsPressed(SDL_SCANCODE_LEFT)) {
			if (!IsObstacle(Maths::IVec2D(-18, 0)) &&
				!IsChair(Maths::IVec2D(-18, 0)) &&
				!IsOccupedByNPC(Maths::IVec2D(-18, 0))) {
				xOffset -= int(2 * speed);
				pNpc->Move(int(2 * speed), 0);
			}
		}

		if (pKbd->KeyIsPressed(SDL_SCANCODE_LCTRL)) {
			if (IsOccupedByNPC(pPlayer->GetLookingDirection() * 18)) {
				pPlayer->Talk();
				pNpc->Talk(-pPlayer->GetLookingDirection());
				talkTimer.ResetTimer(1.5f);
			}
		}
	}
	else {
		talkTimer.Update();
		if (talkTimer.IsTimerDown()) {
			pPlayer->StopTalking();
			pNpc->StopTalking();
		}
	}
}

void House::Draw()
{
	for (int i = -1; i < gridWidth; i++) {
		for (int j = -1; j < gridHeight; j++) {
			Maths::IVec2D pos = Maths::IVec2D(i + static_cast<int>((xOffset - 400) / tileWidth), j + static_cast<int>((yOffset - 300) / tileHeight));
			auto itr = tiles.find(pos);
			if (itr != tiles.end()) {
				int tileType = static_cast<int>(itr->second) & (~static_cast<int>(House::TileTypes::Flower));
				tileType &= (~static_cast<int>(House::TileTypes::FlowerPot));
				tileType &= (~static_cast<int>(House::TileTypes::NPCTile));
				Maths::IRect srcRect = Maths::IRect((tileType % 6) * 16, (static_cast<int>(tileType / 6)) * 16, 16, 16);
				int layer = 0;
				if (tileType == static_cast<int>(House::TileTypes::Table00) || tileType == static_cast<int>(House::TileTypes::Table01)) layer = 5;
				pGfx->DrawSprite(Maths::IRect(itr->first.x * tileWidth - xOffset + 400, itr->first.y * tileHeight - yOffset + 300, tileWidth, tileHeight),srcRect, sprite, layer);
				if ((static_cast<int>(itr->second) & static_cast<int>(House::TileTypes::FlowerPot)) == static_cast<int>(House::TileTypes::FlowerPot)) {
					pGfx->DrawSprite(Maths::IRect(itr->first.x * tileWidth - xOffset + 400, itr->first.y * tileHeight - yOffset + 300, tileWidth, tileHeight), Maths::IRect(48, 80, 16, 16), sprite);
				}
				else if ((static_cast<int>(itr->second) & static_cast<int>(House::TileTypes::Flower)) == static_cast<int>(House::TileTypes::Flower)) {
					pGfx->DrawSprite(Maths::IRect(itr->first.x * tileWidth - xOffset + 400, itr->first.y * tileHeight - yOffset + 300, tileWidth, tileHeight), Maths::IRect(64, 80, 16, 16), sprite, 5);
				}
			}
		}
	}
	pNpc->Draw();
	if (pPlayer->IsTalking()) {
		text.Draw("Hello  !", BLACK, GRAY, WHITE);
	}
}

bool House::GoOutside() const
{
	House::TileTypes currentTile = GetCurrentTile();
	if ((currentTile == House::TileTypes::Carpet0 ||
		currentTile == House::TileTypes::Carpet1) && tiles.find(Maths::IVec2D(static_cast<int>(xOffset / tileWidth), static_cast<int>((yOffset + 18) / tileHeight))) == tiles.end()) {
		return pKbd->KeyIsPressed(SDL_SCANCODE_DOWN);
	}
	return false;
}

House::TileTypes House::GetCurrentTile() const
{
	auto itr = tiles.find(Maths::IVec2D(static_cast<int>(xOffset / tileWidth), static_cast<int>(yOffset / tileHeight)));
	if (itr != tiles.end()) {
		return House::TileTypes(itr->second);
	}
	return TileTypes();
}

bool House::IsObstacle(Maths::IVec2D nextPos) const
{
	Maths::IVec2D pos = Maths::IVec2D(static_cast<int>((nextPos.x + xOffset) / tileWidth), static_cast<int>((nextPos.y + yOffset) / tileHeight));
	auto itr = tiles.find(pos);
	if (itr != tiles.end()) {
		return (itr->second & static_cast<int>(House::TileTypes::FlowerPot)) == static_cast<int>(House::TileTypes::FlowerPot) ||
			!((itr->second == static_cast<int>(House::TileTypes::ChairL)) ||
			(itr->second == static_cast<int>(House::TileTypes::ChairR)) ||
			(itr->second == static_cast<int>(House::TileTypes::Carpet0)) ||
			(itr->second == static_cast<int>(House::TileTypes::Carpet1)) ||
			((itr->second & static_cast<int>(House::TileTypes::Floor0)) == static_cast<int>(House::TileTypes::Floor0) ||
			(itr->second & static_cast<int>(House::TileTypes::Floor1)) == static_cast<int>(House::TileTypes::Floor1) ||
			(itr->second & static_cast<int>(House::TileTypes::Floor2)) == static_cast<int>(House::TileTypes::Floor2)));
	}
	return true;
}

bool House::IsChair(Maths::IVec2D nextPos) const
{
	Maths::IVec2D pos = Maths::IVec2D(static_cast<int>((nextPos.x + xOffset) / tileWidth), static_cast<int>((nextPos.y + yOffset) / tileHeight));
	auto itr = tiles.find(pos);
	if (itr != tiles.end()) {
		switch (static_cast<House::TileTypes>(itr->second)) {
		case House::TileTypes::ChairL:
		case House::TileTypes::ChairR:
			return true;
		default:
			return false;
		}
	}
	return false;
}

bool House::IsOccupedByNPC(Maths::IVec2D nextPos) const
{
	Maths::IVec2D pos = Maths::IVec2D(static_cast<int>((nextPos.x + xOffset) / tileWidth), static_cast<int>((nextPos.y + yOffset) / tileHeight));
	Maths::IVec2D npcPos = Maths::IVec2D((pNpc->GetRect().GetCenterOfRect().x - 400 + xOffset) / tileWidth, (pNpc->GetRect().GetCenterOfRect().y - 282 + yOffset) / tileHeight);
	return pos == npcPos;
}
