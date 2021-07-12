#include "Scene.h"

Scene::Scene(const std::string& backgroundPath)
	:
	pGfx(GraphicsEngine::Graphics::GetInstance())
{
	backgroundSprite.InitSurface(backgroundPath.c_str());
}