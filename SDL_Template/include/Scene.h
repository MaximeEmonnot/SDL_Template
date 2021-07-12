#pragma once
#include "Player.h"

class Scene {
public:
	enum class SceneType {
		TitleScene,
		ChoosingScene,
		ExplorationScene,
		FightingScene,
		Null
	};
public:
	Scene(const std::string& backgroundPath, Scene::SceneType type);

	virtual void Update() = 0;
	virtual void Draw() = 0;

	bool ChangeScene() const;
	SceneType GetCurrentScene() const;
	SceneType GetNextScene() const;

protected:
	std::shared_ptr<GraphicsEngine::Graphics> pGfx;
	GraphicsEngine::Sprite backgroundSprite;
	bool bWillChangeScene = false;
	SceneType currentScene;
	SceneType newScene = SceneType::Null;
};