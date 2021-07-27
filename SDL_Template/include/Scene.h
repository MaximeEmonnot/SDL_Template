#pragma once
#include "Player.h"
#include "Font.h"
#include "TextBox.h"

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
	Scene(Scene::SceneType type);
	~Scene();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	bool ChangeScene() const;
	SceneType GetCurrentScene() const;
	SceneType GetNextScene() const;

protected:
	std::shared_ptr<GraphicsEngine::Font> pFont;
	std::shared_ptr<GraphicsEngine::Graphics> pGfx;
	SceneType currentScene;
	SceneType newScene = SceneType::Null;
	bool bWillChangeScene = false;
};