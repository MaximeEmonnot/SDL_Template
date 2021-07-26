#include "Scene.h"

Scene::Scene(Scene::SceneType type)
	:
	pFont(GraphicsEngine::Font::GetInstance("ttf/arcadeClassic.TTF", 16)),
	pGfx(GraphicsEngine::Graphics::GetInstance()),
	currentScene(type)
{
}

void Scene::Draw()
{
}

Scene::~Scene()
{
	pFont->Kill();
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
