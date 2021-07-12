#pragma once
#include "Player.h"

class Scene {
public:
	enum class SceneType {
		TitleScene,
		ChoosingScene,
		ExplorationScene,
		FightingScene
	};
public:
	Scene(const std::string& backgroundPath);

	virtual void Update(int& output) = 0;
	virtual void Draw() = 0;

protected:
	std::shared_ptr<GraphicsEngine::Graphics> pGfx;
	GraphicsEngine::Sprite backgroundSprite;
};