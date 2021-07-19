#include "ExplorationScene.h"

ExplorationScene::ExplorationScene(const std::string& backgroundPath)
	:
	Scene(backgroundPath, Scene::SceneType::ExplorationScene),
	pGrid(Grid::GetInstance()),
	pPlayer(Player::GetInstance(Maths::IRect(384, 284, 32, 32), "json/kirby.json")),
	pTimer(CoreSystem::Timer::GetInstance()),
	pKbd(CoreSystem::Keyboard::GetInstance())
{
}

ExplorationScene::~ExplorationScene()
{
	pGrid->Kill();
}

void ExplorationScene::Update()
{
	pGrid->Update();

	bWillChangeScene = false;
	pPlayer->Update(pTimer->DeltaTime());
	pPlayer->Move();

	if (pGrid->PlayerTriggersFight() && !pPlayer->GetPokemon().IsDead()) {
		bWillChangeScene = true;
		newScene = Scene::SceneType::FightingScene;
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_H)) {
		pPlayer->GetPokemon().Heal();
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_S)) {
		pPlayer->SaveJSON();
	}
}

void ExplorationScene::Draw()
{
	//pGfx->DrawSprite(backgroundSprite);
	pGrid->Draw();
	pPlayer->Draw();
	pPlayer->DrawPokemon();

	font.DrawText(Maths::IVec2D(10, 10), (std::string("X     ") + std::to_string(pGrid->GetWorldPosition().x) + "\n" + std::string("Y     ") + std::to_string(pGrid->GetWorldPosition().y)).c_str(), RED);
}
