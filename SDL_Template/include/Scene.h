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
	void ClearKbd();
	void ClearMouse();

protected:
	std::shared_ptr<GraphicsEngine::Font> pFont;
	std::shared_ptr<GraphicsEngine::Graphics> pGfx;
	std::shared_ptr<CoreSystem::Mouse> pMouse;
	std::shared_ptr<CoreSystem::Keyboard> pKbd;
	std::shared_ptr<CoreSystem::Timer> pTimer;
	std::shared_ptr<CoreSystem::Window> pWnd;
	std::shared_ptr<CoreSystem::ThreadPool> pThread;
	std::shared_ptr<SoundEngine::SoundSystem> pSoundSystem;
	std::shared_ptr<Player> pPlayer;
	SceneType currentScene;
	SceneType newScene = SceneType::Null;
	bool bWillChangeScene = false;
};