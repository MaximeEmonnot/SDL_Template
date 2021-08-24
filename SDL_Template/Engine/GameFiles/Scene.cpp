#include "Scene.h"

Scene::Scene(std::shared_ptr<Player> pPlayer, Scene::SceneType type)
	:
	pPlayer(pPlayer),
	currentScene(type)
{
}

bool Scene::ChangeScene() const
{
	return bWillChangeScene;
}

Scene::SceneType Scene::GetNextScene() const
{
	return newScene;
}

Scene::SceneType Scene::GetCurrentScene() const
{
	return currentScene;
}

void Scene::ClearKbd()
{
	CoreSystem::Keyboard::GetInstance().ReadChar();
	CoreSystem::Keyboard::GetInstance().ReadKey();
}

void Scene::ClearMouse()
{
	CoreSystem::Mouse::GetInstance().Read();
}
