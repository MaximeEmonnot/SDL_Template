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
	Scene(std::shared_ptr<Player> pPlayer, Scene::SceneType type);

	virtual void Update() = 0;
	virtual void Draw() = 0;

	bool ChangeScene() const;
	SceneType GetCurrentScene() const;
	SceneType GetNextScene() const;
protected:
	void ClearKbd();
	void ClearMouse();

protected:
	std::shared_ptr<Player> pPlayer;
	SceneType currentScene;
	SceneType newScene = SceneType::Null;
	bool bWillChangeScene = false;
};