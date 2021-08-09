#include "Scene.h"

Scene::Scene(Scene::SceneType type)
	:
	pFont(GraphicsEngine::Font::GetInstance("ttf/arcadeClassic.TTF", 16)),
	pGfx(GraphicsEngine::Graphics::GetInstance()),
	pMouse(CoreSystem::Mouse::GetInstance()),
	pKbd(CoreSystem::Keyboard::GetInstance()),
	pTimer(CoreSystem::Timer::GetInstance()),
	pWnd(CoreSystem::Window::GetInstance()),
	pSoundSystem(SoundEngine::SoundSystem::GetInstance()),
	pPlayer(Player::GetInstance(Maths::IRect(384, 267, 32, 44), "json/player.json")),
	currentScene(type)
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

void Scene::ClearKbd()
{
	pKbd->ReadChar();
	pKbd->ReadKey();
}

void Scene::ClearMouse()
{
	pMouse->Read();
}
