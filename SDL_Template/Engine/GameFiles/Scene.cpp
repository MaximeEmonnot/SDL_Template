#include "Scene.h"

Scene::Scene(const std::string& backgroundPath, Scene::SceneType type)
	:
	pGfx(GraphicsEngine::Graphics::GetInstance()),
	currentScene(type)
{
	backgroundSprite.InitSurface(backgroundPath.c_str());
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
