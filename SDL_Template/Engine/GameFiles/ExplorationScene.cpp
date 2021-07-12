#include "ExplorationScene.h"

ExplorationScene::ExplorationScene(const std::string& backgroundPath)
	:
	Scene(backgroundPath),
	player(Maths::IRect(10, 10, 32, 32), "json/kirby.json"),
	pTimer(CoreSystem::Timer::GetInstance())
{
}

void ExplorationScene::Update(int& output)
{
	player.Update(pTimer->DeltaTime());
	player.Move();
}

void ExplorationScene::Draw()
{
	pGfx->DrawSprite(backgroundSprite);
	player.Draw();
}
