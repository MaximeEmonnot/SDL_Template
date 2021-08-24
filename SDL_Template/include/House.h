#pragma once
#include <unordered_map>

#include "Graphics.h"
#include "TextBox.h"
#include "Player.h"
#include "NPC.h"

class House {
public:
	enum class TileTypes {
		Table00 = 0,
		Table01 = 1,
		Table10 = 2,
		Table11 = 3,
		ChairL = 4,
		ChairR = 5,
		Closet00 = 6,
		Closet01 = 7,
		Closet10 = 8,
		Closet11 = 9,
		Closet20 = 10,
		Closet21 = 11,
		Wall0 = 12,
		Wall1 = 13,
		WallWindow0 = 14,
		WallWindow1 = 15,
		Carpet0 = 16,
		Carpet1 = 17,
		Sink00 = 18,
		Sink01 = 19,
		Sink10 = 20,
		Sink11 = 21,
		Drawer0 = 22,
		Drawer1 = 23,
		Fridge0 = 24,
		Fridge1 = 25,
		Fridge2 = 26,
		Tv0 = 27,
		Tv1 = 28,
		Tv2 = 29,
		Floor0 = 30,
		Floor1 = 31,
		Floor2 = 32,
		FlowerPot = 64,
		Flower = 128,
		NPCTile = 256
	};
public:
	House(std::shared_ptr<Player> pPlayer);

	void Update(float speed);
	void Draw();

	bool GoOutside() const;

private:
	TileTypes GetCurrentTile() const;
	bool IsObstacle(Maths::IVec2D nextPos) const;
	bool IsChair(Maths::IVec2D nextPos) const;
	bool IsOccupedByNPC(Maths::IVec2D nextPos) const;
private:
	GraphicsEngine::Sprite sprite;
	std::unique_ptr<NPC> pNpc;
	std::shared_ptr<Player> pPlayer;

	std::unordered_map<Maths::IVec2D, int, Maths::IVec2D::Hash> tiles;

	TimerManager talkTimer;
	TextBox text;

	const int tileWidth = 32;
	const int tileHeight = 32;
	const int gridHeight = 19;
	const int gridWidth = 25;

	short xOffset = 160;
	short yOffset = 242;
};