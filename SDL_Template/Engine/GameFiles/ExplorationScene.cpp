#include "ExplorationScene.h"

ExplorationScene::ExplorationScene(const std::string& backgroundPath)
	:
	Scene(backgroundPath, Scene::SceneType::ExplorationScene),
	pPlayer(Player::GetInstance(Maths::IRect(10, 10, 32, 32), "json/kirby.json")),
	pTimer(CoreSystem::Timer::GetInstance()),
	pKbd(CoreSystem::Keyboard::GetInstance())
{
}

void ExplorationScene::Update()
{
	bWillChangeScene = false;
	pPlayer->Update(pTimer->DeltaTime());
	pPlayer->Move();

	if (pKbd->KeyIsPressed(SDL_SCANCODE_F) && !pPlayer->GetPokemon().IsDead()) {
		bWillChangeScene = true;
		newScene = Scene::SceneType::FightingScene;
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_H)) {
		pPlayer->GetPokemon().Heal();
	}
}

void ExplorationScene::Draw()
{
	pGfx->DrawSprite(backgroundSprite);
	pPlayer->Draw();
	pPlayer->DrawPokemon();
}
