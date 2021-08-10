#include "House.h"

House::House()
	:
	pGfx(GraphicsEngine::Graphics::GetInstance()),
	pKbd(CoreSystem::Keyboard::GetInstance()),
	pTimer(CoreSystem::Timer::GetInstance()),
	pPlayer(Player::GetInstance(Maths::IRect(384, 267, 32, 44), "json/player.json")),
	text(Maths::IRect(Maths::IRect(25, 500, 200, 75)))
{
	//Sprite initialisation
	sprite.InitSurface("Images/houseTileSheet.png");

	//Init Tiles

	JSONParser::Reader jsonReader;

	jsonReader.ReadFile("json/houseTiles.json");

	auto& tileInfo = jsonReader.GetValueOf("Tiles");
	for (auto itr = tileInfo.MemberBegin(); itr != tileInfo.MemberEnd(); ++itr) {
		int tileValue = (itr->value.GetArray()[2].GetInt() & (~TileTypes::NPCTile));
		tiles.insert(std::pair<Maths::IVec2D, TileTypes>(Maths::IVec2D(itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt()), TileTypes(tileValue)));
		if ((itr->value.GetArray()[2].GetInt() & TileTypes::NPCTile) == TileTypes::NPCTile) {
			pNpc = std::make_unique<NPC>(Maths::IRect(itr->value.GetArray()[0].GetInt() * tileWidth - xOffset + 400, itr->value.GetArray()[1].GetInt() * tileHeight - yOffset + 282, 32, 44), Maths::IVec2D(itr->value.GetArray()[0].GetInt() * tileWidth, itr->value.GetArray()[1].GetInt() * tileHeight), "json/npc.json", std::make_unique<AIStanding>());
		}
	}
}

void House::Update()
{
	pNpc->Update(pTimer->DeltaTime());
	pNpc->UpdateAI();

	if (!pPlayer->IsTalking()) {
		if (pKbd->KeyIsPressed(SDL_SCANCODE_UP)) {
			if (!IsObstacle(Maths::IVec2D(0, -18)) &&
				!IsOccupedByNPC(Maths::IVec2D(0, -18)))
			{
				yOffset -= 2;
				pNpc->Move(0, 2);
			}
		}
		if (pKbd->KeyIsPressed(SDL_SCANCODE_RIGHT)) {
			if (!IsObstacle(Maths::IVec2D(18, 0)) &&
				!IsChair(Maths::IVec2D(18, 0)) &&
				!IsOccupedByNPC(Maths::IVec2D(18, 0))) {
				xOffset += 2;
				pNpc->Move(-2, 0);
			}
		}
		if (pKbd->KeyIsPressed(SDL_SCANCODE_DOWN)) {
			if (!IsObstacle(Maths::IVec2D(0, 18)) &&
				!IsOccupedByNPC(Maths::IVec2D(0, 18))) {
				yOffset += 2;
				pNpc->Move(0, -2);
			}
		}
		if (pKbd->KeyIsPressed(SDL_SCANCODE_LEFT)) {
			if (!IsObstacle(Maths::IVec2D(-18, 0)) &&
				!IsChair(Maths::IVec2D(-18, 0)) &&
				!IsOccupedByNPC(Maths::IVec2D(-18, 0))) {
				xOffset -= 2;
				pNpc->Move(2, 0);
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
			Maths::IVec2D pos = Maths::IVec2D(i + int((xOffset - 400) / tileWidth), j + int((yOffset - 300) / tileHeight));
			auto itr = tiles.find(pos);
			if (itr != tiles.end()) {
				int tileType = itr->second & (~House::TileTypes::Flower);
				tileType &= (~House::TileTypes::FlowerPot);
				tileType &= (~House::TileTypes::NPCTile);
				Maths::IRect srcRect = Maths::IRect((tileType % 6) * 16, (int(tileType / 6)) * 16, 16, 16);
				pGfx->DrawSprite(Maths::IRect(itr->first.x * tileWidth - xOffset + 400, itr->first.y * tileHeight - yOffset + 300, tileWidth, tileHeight),srcRect, sprite);
				if ((itr->second & House::TileTypes::FlowerPot) == House::TileTypes::FlowerPot) {
					pGfx->DrawSprite(Maths::IRect(itr->first.x * tileWidth - xOffset + 400, itr->first.y * tileHeight - yOffset + 300, tileWidth, tileHeight), Maths::IRect(48, 80, 16, 16), sprite);
				}
				else if ((itr->second & House::TileTypes::Flower) == House::TileTypes::Flower) {
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
		currentTile == House::TileTypes::Carpet1) && tiles.find(Maths::IVec2D(int(xOffset / tileWidth), int((yOffset + 18) / tileHeight))) == tiles.end()) {
		return pKbd->KeyIsPressed(SDL_SCANCODE_DOWN);
	}
	return false;
}

House::TileTypes House::GetCurrentTile() const
{
	auto itr = tiles.find(Maths::IVec2D(int(xOffset / tileWidth), int(yOffset / tileHeight)));
	if (itr != tiles.end()) {
		return House::TileTypes(itr->second);
	}
	return TileTypes();
}

bool House::IsObstacle(Maths::IVec2D nextPos) const
{
	Maths::IVec2D pos = Maths::IVec2D(int((nextPos.x + xOffset) / tileWidth), int((nextPos.y + yOffset) / tileHeight));
	auto itr = tiles.find(pos);
	if (itr != tiles.end()) {
		return (itr->second & House::TileTypes::FlowerPot) == House::TileTypes::FlowerPot ||
			//(itr->second & House::TileTypes::Flower) == House::TileTypes::Flower ||
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

bool House::IsChair(Maths::IVec2D nextPos) const
{
	Maths::IVec2D pos = Maths::IVec2D((nextPos.x + xOffset) / tileWidth, (nextPos.y + yOffset) / tileHeight);
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

bool House::IsOccupedByNPC(Maths::IVec2D nextPos) const
{
	Maths::IVec2D pos = Maths::IVec2D((nextPos.x + xOffset) / tileWidth, (nextPos.y + yOffset) / tileHeight);
	Maths::IVec2D npcPos = Maths::IVec2D((pNpc->GetRect().GetCenterOfRect().x - 400 + xOffset) / tileWidth, (pNpc->GetRect().GetCenterOfRect().y - 282 + yOffset) / tileHeight);
	return pos == npcPos;
}
