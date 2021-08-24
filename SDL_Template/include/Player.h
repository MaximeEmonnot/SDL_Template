#pragma once
#include "NetworkSystem.h"
#include "Character.h"
#include "Keyboard.h"
#include "Pokemon.h"

#include "Consumable.h"
#include "Ball.h"
#include <map>


class Player : public Character {
private:
	friend class ExplorationScene;
private:
	enum class AnimationList {
		WalkingLeft,
		WalkingUp,
		WalkingRight,
		WalkingDown,
		StandingLeft,
		StandingUp,
		StandingRight,
		StandingDown,
		RunningLeft,
		RunningUp,
		RunningRight,
		RunningDown,
		BikingLeft,
		BikingUp,
		BikingRight,
		BikingDown,
		StandingBikeLeft,
		StandingBikeUp,
		StandingBikeRight,
		StandingBikeDown,
		OnWaterLeft,
		OnWaterUp,
		OnWaterRight,
		OnWaterDown
	};

	enum class LocomotionState {
		Walking,
		Running,
		Biking,
		OnWater
	};
public:
	Player(Maths::IRect rect, const std::string& animFile);
	~Player() override;

	void Move();

	void ConnectAs(bool type);
	bool IsOnline() const;

	void AddPokemon(Pokemon& pkmn);
	void SetFirstPokemon(int index);
	Pokemon& GetPokemon();
	std::vector<Pokemon> GetPokemonList() const;

	Maths::IVec2D GetLookingDirection() const;

	void DrawPokemon();

	void Talk();
	void StopTalking();

	void UpdateBike();
	void OnWater(bool value);
	bool IsOnWater() const;
	LocomotionState GetLocomotionState() const;
	int GetAnimation() const;

	bool IsTalking() const;

	void UsePokemon(int index);
	void OnUseSuccess(bool value);

	bool CapturePokemon(int index, Pokemon& pkmn);

	void PickUpItem(std::shared_ptr<Item> item);
	void UseItem(int indexItem) {
		UseItem(indexItem, int(selectedPokemon - pokemon.begin()));
	}
	void UseItem(int indexItem, int indexPkmn);

	bool CanUseItem(int index);

	bool IsUsingSpecial() const;
	bool SpecialIsSuccessful() const;
	Pokemon::Type GetUsedPokemonType();

	std::map<std::shared_ptr<Item>, int> GetItemList() const;

private:

	void InitFromJSON();
	void SaveToJSON();

public:
	bool TEST_bInitFromJSON = false;

private:
	bool bIsOnline = false;
	bool bIsHost = false;

	Pokemon::Type usedPkmnType = Pokemon::Type::Normal;
	bool bIsUsingSpecial = false;
	bool bSpecialIsSuccessful = false;
	bool bIsTalking = false;

	LocomotionState locomotion = LocomotionState::Walking;

	std::vector<Pokemon>::iterator selectedPokemon;

	std::vector<Pokemon> pokemon;

	std::map<std::shared_ptr<Item>, int> items;
	Maths::IVec2D velocity;
	Maths::IVec2D lookingDirection;
	float speed = 2.0f;
};