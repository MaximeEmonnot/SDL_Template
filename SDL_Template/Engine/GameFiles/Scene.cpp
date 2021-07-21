#include "Scene.h"

Scene::Scene(Scene::SceneType type)
	:
	pGfx(GraphicsEngine::Graphics::GetInstance()),
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
