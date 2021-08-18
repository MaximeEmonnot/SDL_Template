#include "Player.h"
#include <random>

Player::Player(Maths::IRect rect, const std::string& animFile)
	:
	Character(rect),
	pKbd(CoreSystem::Keyboard::GetInstance())
{
	JSONParser::Reader jsonParse;
	jsonParse.ReadFile(animFile);

	mSprite = GraphicsEngine::Sprite(jsonParse.GetValueOf("filename").GetString());

	auto& v = jsonParse.GetValueOf("animations");
	for (auto itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr) {
		mAnimations.push_back(GraphicsEngine::Animation({ itr->value.GetArray()[0].GetInt(),
														itr->value.GetArray()[1].GetInt(),
														itr->value.GetArray()[2].GetInt(),
														itr->value.GetArray()[3].GetInt() },
			itr->value.GetArray()[4].GetInt(),
			mSprite,
			itr->value.GetArray()[5].GetFloat()));
	}

	miCurSequence = static_cast<int>(AnimationList::StandingRight);
}

Player::~Player()
{
	printf("Player destructor !\n");
	items.clear();
}

void Player::InitFromJSON()
{
	JSONParser::Reader jsonReaderSF;
	jsonReaderSF.ReadFile("json/saveFile.json");

	//Init consumables
	if (jsonReaderSF.IsValueAvailable("Consumables")) {
		auto& consumableValue = jsonReaderSF.GetValueOf("Consumables");
		for (auto itr = consumableValue.MemberBegin(); itr != consumableValue.MemberEnd(); ++itr) {
			items.insert(std::pair<std::shared_ptr<Item>, int>(std::make_shared<Consumable>(itr->name.GetString(), itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt()), itr->value.GetArray()[2].GetInt()));
		}
	}

	//Init balls
	if (jsonReaderSF.IsValueAvailable("Balls")) {
		auto& ballValue = jsonReaderSF.GetValueOf("Balls");
		for (auto itr = ballValue.MemberBegin(); itr != ballValue.MemberEnd(); ++itr) {
			items.insert(std::pair<std::shared_ptr<Item>, int>(std::make_shared<Ball>(itr->name.GetString(), itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt()), itr->value.GetArray()[2].GetInt()));
		}
	}


	// NEW
	// Lead pkmn
	{
		auto& selectedPkmnValue = jsonReaderSF.GetValueOf("Pokemon0");
		Pokemon newPkmn;
		newPkmn.name = selectedPkmnValue.FindMember("Name")->value.GetString();
		newPkmn.hp = selectedPkmnValue.FindMember("PokemonCharacteristics")->value.GetArray()[0].GetInt();
		newPkmn.currentMaxHP = selectedPkmnValue.FindMember("PokemonCharacteristics")->value.GetArray()[1].GetInt();
		newPkmn.att = selectedPkmnValue.FindMember("PokemonCharacteristics")->value.GetArray()[2].GetInt();
		newPkmn.def = selectedPkmnValue.FindMember("PokemonCharacteristics")->value.GetArray()[3].GetInt();
		newPkmn.lvl = selectedPkmnValue.FindMember("PokemonCharacteristics")->value.GetArray()[4].GetInt();
		newPkmn.id = selectedPkmnValue.FindMember("PokemonCharacteristics")->value.GetArray()[5].GetInt();
		newPkmn.pGfx = GraphicsEngine::Graphics::GetInstance();
		switch (newPkmn.id) {
		case 1:
			newPkmn.type = Pokemon::Type::Grass;
			newPkmn.sprite = GraphicsEngine::Sprite("Images/bulbasaur.png");
			break;
		case 2:
			newPkmn.type = Pokemon::Type::Fire;
			newPkmn.sprite = GraphicsEngine::Sprite("Images/charmander.png");
			break;
		case 3:
			newPkmn.type = Pokemon::Type::Water;
			newPkmn.sprite = GraphicsEngine::Sprite("Images/squirttle.png");
			break;
		default:
			break;
		}
		for (int iAttack = 0; iAttack < 4; iAttack++) {
			if (selectedPkmnValue.HasMember(("Ability" + std::to_string(iAttack) + "Name").c_str())) {
				Pokemon::Ability ablty(
					selectedPkmnValue.FindMember(("Ability" + std::to_string(iAttack) + "Name").c_str())->value.GetString(),
					selectedPkmnValue.FindMember(("Ability" + std::to_string(iAttack) + "Characteristics").c_str())->value.GetArray()[0].GetInt(),
					selectedPkmnValue.FindMember(("Ability" + std::to_string(iAttack) + "Characteristics").c_str())->value.GetArray()[1].GetInt(),
					static_cast<Pokemon::Type>(selectedPkmnValue.FindMember(("Ability" + std::to_string(iAttack) + "Characteristics").c_str())->value.GetArray()[3].GetInt()));
				ablty.SetPP(selectedPkmnValue.FindMember(("Ability" + std::to_string(iAttack) + "Characteristics").c_str())->value.GetArray()[2].GetInt());
				newPkmn.LoadAbility(ablty);
			}
			else break;
		}
		AddPokemon(newPkmn);
	}
	//Other pkmns
	for (int iPkmn = 1; iPkmn < 6; iPkmn++) {
		if (jsonReaderSF.IsValueAvailable("Pokemon" + std::to_string(iPkmn))) {
			auto& pkmnValue = jsonReaderSF.GetValueOf("Pokemon" + std::to_string(iPkmn));
			Pokemon newPkmn;
			newPkmn.name = pkmnValue.FindMember("Name")->value.GetString();
			newPkmn.hp = pkmnValue.FindMember("PokemonCharacteristics")->value.GetArray()[0].GetInt();
			newPkmn.currentMaxHP = pkmnValue.FindMember("PokemonCharacteristics")->value.GetArray()[1].GetInt();
			newPkmn.att = pkmnValue.FindMember("PokemonCharacteristics")->value.GetArray()[2].GetInt();
			newPkmn.def = pkmnValue.FindMember("PokemonCharacteristics")->value.GetArray()[3].GetInt();
			newPkmn.lvl = pkmnValue.FindMember("PokemonCharacteristics")->value.GetArray()[4].GetInt();
			newPkmn.id = pkmnValue.FindMember("PokemonCharacteristics")->value.GetArray()[5].GetInt();
			newPkmn.pGfx = GraphicsEngine::Graphics::GetInstance();
			switch (newPkmn.id) {
			case 1:
				newPkmn.type = Pokemon::Type::Grass;
				newPkmn.sprite = GraphicsEngine::Sprite("Images/bulbasaur.png");
				break;
			case 2:
				newPkmn.type = Pokemon::Type::Fire;
				newPkmn.sprite = GraphicsEngine::Sprite("Images/charmander.png");
				break;
			case 3:
				newPkmn.type = Pokemon::Type::Water;
				newPkmn.sprite = GraphicsEngine::Sprite("Images/squirttle.png");
				break;
			default:
				break;
			}
			for (int iAttack = 0; iAttack < 4; iAttack++) {
				if (pkmnValue.HasMember(("Ability" + std::to_string(iAttack) + "Name").c_str())) {
					Pokemon::Ability ablty(
						pkmnValue.FindMember(("Ability" + std::to_string(iAttack) + "Name").c_str())->value.GetString(),
						pkmnValue.FindMember(("Ability" + std::to_string(iAttack) + "Characteristics").c_str())->value.GetArray()[0].GetInt(),
						pkmnValue.FindMember(("Ability" + std::to_string(iAttack) + "Characteristics").c_str())->value.GetArray()[1].GetInt(),
						static_cast<Pokemon::Type>(pkmnValue.FindMember(("Ability" + std::to_string(iAttack) + "Characteristics").c_str())->value.GetArray()[3].GetInt()));
					ablty.SetPP(pkmnValue.FindMember(("Ability" + std::to_string(iAttack) + "Characteristics").c_str())->value.GetArray()[2].GetInt());
					newPkmn.LoadAbility(ablty);
				}
				else break;
			}
			AddPokemon(newPkmn);
		}
		else break;
	}
}

void Player::SaveToJSON()
{
	JSONParser::Writer jsonWriter;

	//Save Inventory
	for (auto& entry : items) {
		
		std::shared_ptr<Consumable> consumable = std::dynamic_pointer_cast<Consumable, Item>(entry.first);
		if (consumable != nullptr) {
			jsonWriter.AddObjectMember("Consumables", consumable->GetName(), consumable->GetID(), consumable->GetBonusValue(), entry.second);
		}
		else {
			std::shared_ptr<Ball> ball = std::dynamic_pointer_cast<Ball, Item>(entry.first);
			if (ball != nullptr) {
				jsonWriter.AddObjectMember("Balls", ball->GetName(), ball->GetID(), ball->GetProbability(), entry.second);
			}
		}
	}

	// NEW
	// Save selected pkmn
	jsonWriter.AddObjectStringMember("Pokemon0", "Name", selectedPokemon->name); 
	jsonWriter.AddObjectMember("Pokemon0", "PokemonCharacteristics", selectedPokemon->hp, selectedPokemon->currentMaxHP, selectedPokemon->att, selectedPokemon->def, selectedPokemon->lvl, selectedPokemon->id);
	int iAttack = 0;
	for (auto attack : selectedPokemon->GetAbilities()) {
		jsonWriter.AddObjectStringMember("Pokemon0", "Ability" + std::to_string(iAttack) + "Name", attack.GetName());
		jsonWriter.AddObjectMember("Pokemon0", "Ability" + std::to_string(iAttack) + "Characteristics", attack.GetPower(), attack.GetPP(), attack.GetMaxPP(), static_cast<int>(attack.GetType()));
		iAttack++;
	}

	//Save Pokemons
	int iPkmn = 1;
	for (auto& pkmn : pokemon) {
		iAttack = 0;
		if (pkmn != *selectedPokemon) {
			jsonWriter.AddObjectStringMember("Pokemon" + std::to_string(iPkmn), "Name", pkmn.name);
			jsonWriter.AddObjectMember("Pokemon" + std::to_string(iPkmn), "PokemonCharacteristics", pkmn.hp, pkmn.currentMaxHP, pkmn.att, pkmn.def, pkmn.lvl, pkmn.id);
			for (auto attack : pkmn.GetAbilities()) {
				jsonWriter.AddObjectStringMember("Pokemon" + std::to_string(iPkmn), "Ability" + std::to_string(iAttack) + "Name", attack.GetName());
				jsonWriter.AddObjectMember("Pokemon" + std::to_string(iPkmn), "Ability" + std::to_string(iAttack) + "Characteristics", attack.GetPower(), attack.GetPP(), attack.GetMaxPP(), static_cast<int>(attack.GetType()));
				iAttack++;
			}
			iPkmn++;
		}
	}
	jsonWriter.SaveJsonAt("json/saveFile.json");
}

void Player::Move()
{
	Maths::IVec2D dir;
	if (pKbd->KeyIsPressed(SDL_SCANCODE_LEFT)) {
		dir.x -= 1;
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_UP)) {
		dir.y -= 1;
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_RIGHT)) {
		dir.x += 1;
	}
	if (pKbd->KeyIsPressed(SDL_SCANCODE_DOWN)) {
		dir.y += 1;
	}
	
	//Animation
	if (locomotion != LocomotionState::Biking && locomotion != LocomotionState::OnWater) {
		if (pKbd->KeyIsPressed(SDL_SCANCODE_LSHIFT)) locomotion = LocomotionState::Running;
		else locomotion = LocomotionState::Walking;
	}

	if (dir != Maths::IVec2D(0,0)) {
		if (dir.y != 0) {
			if (dir.y > 0) {
				switch (locomotion) {
				case LocomotionState::Walking:
					miCurSequence = static_cast<int>(AnimationList::WalkingDown);
					break;
				case LocomotionState::Running:
					miCurSequence = static_cast<int>(AnimationList::RunningDown);
					break;
				case LocomotionState::Biking:
					miCurSequence = static_cast<int>(AnimationList::BikingDown);
					break;
				case LocomotionState::OnWater:
					miCurSequence = static_cast<int>(AnimationList::OnWaterDown);
					break;
				default:
					break;
				}
				lookingDirection = Maths::IVec2D(0, 1);
			}
			else {
				switch (locomotion) {
				case LocomotionState::Walking:
					miCurSequence = static_cast<int>(AnimationList::WalkingUp);
					break;
				case LocomotionState::Running:
					miCurSequence = static_cast<int>(AnimationList::RunningUp);
					break;
				case LocomotionState::Biking:
					miCurSequence = static_cast<int>(AnimationList::BikingUp);
					break;
				case LocomotionState::OnWater:
					miCurSequence = static_cast<int>(AnimationList::OnWaterUp);
					break;
				default:
					break;
				}
				lookingDirection = Maths::IVec2D(0, -1);
			}
		}
		else {
			if (dir.x > 0) {
				switch (locomotion) {
				case LocomotionState::Walking:
					miCurSequence = static_cast<int>(AnimationList::WalkingRight);
					break;
				case LocomotionState::Running:
					miCurSequence = static_cast<int>(AnimationList::RunningRight);
					break;
				case LocomotionState::Biking:
					miCurSequence = static_cast<int>(AnimationList::BikingRight);
					break;
				case LocomotionState::OnWater:
					miCurSequence = static_cast<int>(AnimationList::OnWaterRight);
					break;
				default:
					break;
				}
				lookingDirection = Maths::IVec2D(1, 0);
			}
			else {
				switch (locomotion) {
				case LocomotionState::Walking:
					miCurSequence = static_cast<int>(AnimationList::WalkingLeft);
					break;
				case LocomotionState::Running:
					miCurSequence = static_cast<int>(AnimationList::RunningLeft);
					break;
				case LocomotionState::Biking:
					miCurSequence = static_cast<int>(AnimationList::BikingLeft);
					break;
				case LocomotionState::OnWater:
					miCurSequence = static_cast<int>(AnimationList::OnWaterLeft);
					break;
				default:
					break;
				}
				lookingDirection = Maths::IVec2D(-1, 0);
			}
		}
	}
	else {
		if (velocity != Maths::IVec2D(0, 0)) {
			if (velocity.y != 0) {
				if (velocity.y > 0) {
					switch (locomotion) {
					case LocomotionState::Walking:
					case LocomotionState::Running:
						miCurSequence = static_cast<int>(AnimationList::StandingDown);
						break;
					case LocomotionState::Biking:
						miCurSequence = static_cast<int>(AnimationList::StandingBikeDown);
						break;
					case LocomotionState::OnWater:
						miCurSequence = static_cast<int>(AnimationList::OnWaterDown);
						break;
					default:
						break;
					}
				}
				else {
					switch (locomotion) {
					case LocomotionState::Walking:
					case LocomotionState::Running:
						miCurSequence = static_cast<int>(AnimationList::StandingUp);
						break;
					case LocomotionState::Biking:
						miCurSequence = static_cast<int>(AnimationList::StandingBikeUp);
						break;
					case LocomotionState::OnWater:
						miCurSequence = static_cast<int>(AnimationList::OnWaterUp);
						break;
					default:
						break;
					}
				}
			}
			else {
				if (velocity.x > 0) {
					switch (locomotion) {
					case LocomotionState::Walking:
					case LocomotionState::Running:
						miCurSequence = static_cast<int>(AnimationList::StandingRight);
						break;
					case LocomotionState::Biking:
						miCurSequence = static_cast<int>(AnimationList::StandingBikeRight);
						break;
					case LocomotionState::OnWater:
						miCurSequence = static_cast<int>(AnimationList::OnWaterRight);
						break;
					default:
						break;
					}
				}
				else {
					switch (locomotion) {
					case LocomotionState::Walking:
					case LocomotionState::Running:
						miCurSequence = static_cast<int>(AnimationList::StandingLeft);
						break;
					case LocomotionState::Biking:
						miCurSequence = static_cast<int>(AnimationList::StandingBikeLeft);
						break;
					case LocomotionState::OnWater:
						miCurSequence = static_cast<int>(AnimationList::OnWaterLeft);
						break;
					default:
						break;
					}
				}
			}
		}
	}

	//Current Velocity
	velocity = dir;
}

void Player::ConnectAs(bool bIsClient)
{	
	if (bIsClient) {
		pNet = Network::NetworkSystem::GetInstance("M4xH3rO3s", 222, 333);
	}
	else {
		bIsHost = true;
		pNet = Network::NetworkSystem::GetInstance("M4xH3rO3s", 333, 222);
	}
}

std::shared_ptr<Network::NetworkSystem> Player::GetNetSystem()
{
	return pNet;
}

void Player::AddPokemon(Pokemon& pkmn)
{
	pokemon.push_back(pkmn);
	selectedPokemon = pokemon.begin();
}

void Player::SetFirstPokemon(int index)
{
	selectedPokemon = std::next(pokemon.begin(), index);
}

Pokemon& Player::GetPokemon()
{
	return *selectedPokemon;
}

std::vector<Pokemon> Player::GetPokemonList() const
{
	return pokemon;
}

Maths::IVec2D Player::GetLookingDirection() const
{
	return lookingDirection;
}

void Player::DrawPokemon()
{
	if (pKbd->KeyIsPressed(SDL_SCANCODE_P)) {
		selectedPokemon->DrawFrontSprite(Maths::IRect(250, 250, 128, 128));
	}
}

void Player::Talk()
{
	bIsTalking = true;
	locomotion = LocomotionState::Walking;

	if (lookingDirection == Maths::IVec2D(0, -1)) {
		miCurSequence = static_cast<int>(AnimationList::StandingUp);
	}
	if (lookingDirection == Maths::IVec2D(1, 0)) {
		miCurSequence = static_cast<int>(AnimationList::StandingRight);
	}
	if (lookingDirection == Maths::IVec2D(0, 1)) {
		miCurSequence = static_cast<int>(AnimationList::StandingDown);
	}
	if (lookingDirection == Maths::IVec2D(-1, 0)) {
		miCurSequence = static_cast<int>(AnimationList::StandingLeft);
	}
}

void Player::StopTalking()
{
	bIsTalking = false;
}

void Player::UpdateBike()
{
	if (locomotion != LocomotionState::Biking) {
		locomotion = LocomotionState::Biking;
		mRect.rect.x -= 8;
		mRect.rect.y -= 4;
		mRect.rect.w = 48;
		mRect.rect.h = 48;
		mReflectionRect.rect.x -= 8;
		mReflectionRect.rect.y -= 4;
		mReflectionRect.rect.w = 48;
		mReflectionRect.rect.h = 48;

		if (lookingDirection == Maths::IVec2D(0, -1)) {
			miCurSequence = static_cast<int>(AnimationList::StandingBikeUp);
		}
		if (lookingDirection == Maths::IVec2D(1, 0)) {
			miCurSequence = static_cast<int>(AnimationList::StandingBikeRight);
		}
		if (lookingDirection == Maths::IVec2D(0, 1)) {
			miCurSequence = static_cast<int>(AnimationList::StandingBikeDown);
		}
		if (lookingDirection == Maths::IVec2D(-1, 0)) {
			miCurSequence = static_cast<int>(AnimationList::StandingBikeLeft);
		}
	}
	else {
		locomotion = LocomotionState::Walking;
		mRect.rect.x += 8;
		mRect.rect.y += 4;
		mRect.rect.w = 32;
		mRect.rect.h = 44;
		mReflectionRect.rect.x += 8;
		mReflectionRect.rect.y += 4;
		mReflectionRect.rect.w = 32;
		mReflectionRect.rect.h = 44;

		if (lookingDirection == Maths::IVec2D(0, -1)) {
			miCurSequence = static_cast<int>(AnimationList::StandingUp);
		}
		if (lookingDirection == Maths::IVec2D(1, 0)) {
			miCurSequence = static_cast<int>(AnimationList::StandingRight);
		}
		if (lookingDirection == Maths::IVec2D(0, 1)) {
			miCurSequence = static_cast<int>(AnimationList::StandingDown);
		}
		if (lookingDirection == Maths::IVec2D(-1, 0)) {
			miCurSequence = static_cast<int>(AnimationList::StandingLeft);
		}
	}
}

void Player::OnWater(bool value)
{
	if (value) {
		locomotion = Player::LocomotionState::OnWater;
		mRect.rect.x -= 10;
		mRect.rect.y -= 7;
		mRect.rect.w = 52;
		mRect.rect.h = 58;
		mReflectionRect.rect.x -= 10;
		mReflectionRect.rect.y -= 7;
		mReflectionRect.rect.w = 52;
		mReflectionRect.rect.h = 58;

		if (lookingDirection == Maths::IVec2D(0, -1)) {
			miCurSequence = static_cast<int>(AnimationList::OnWaterUp);
		}
		if (lookingDirection == Maths::IVec2D(1, 0)) {
			miCurSequence = static_cast<int>(AnimationList::OnWaterRight);
		}
		if (lookingDirection == Maths::IVec2D(0, 1)) {
			miCurSequence = static_cast<int>(AnimationList::OnWaterDown);
		}
		if (lookingDirection == Maths::IVec2D(-1, 0)) {
			miCurSequence = static_cast<int>(AnimationList::OnWaterLeft);
		}
	}
	else {
		locomotion = Player::LocomotionState::Walking;
		mRect.rect.x += 10;
		mRect.rect.y += 7;
		mRect.rect.w = 32;
		mRect.rect.h = 44;
		mReflectionRect.rect.x += 10;
		mReflectionRect.rect.y += 7;
		mReflectionRect.rect.w = 32;
		mReflectionRect.rect.h = 44;
		if (lookingDirection == Maths::IVec2D(0, -1)) {
			miCurSequence = static_cast<int>(AnimationList::StandingUp);
		}
		if (lookingDirection == Maths::IVec2D(1, 0)) {
			miCurSequence = static_cast<int>(AnimationList::StandingRight);
		}
		if (lookingDirection == Maths::IVec2D(0, 1)) {
			miCurSequence = static_cast<int>(AnimationList::StandingDown);
		}
		if (lookingDirection == Maths::IVec2D(-1, 0)) {
			miCurSequence = static_cast<int>(AnimationList::StandingLeft);
		}
	}
}

bool Player::IsOnWater() const
{
	return locomotion == Player::LocomotionState::OnWater;
}

Player::LocomotionState Player::GetLocomotionState() const
{
	return locomotion;
}

bool Player::IsTalking() const
{
	return bIsTalking;
}

void Player::UsePokemon(int index)
{
	Pokemon pkmn = pokemon.at(index);
	bIsUsingSpecial = true;
	usedPkmnType = pkmn.GetType();
}

void Player::OnUseSuccess(bool value)
{
	bSpecialIsSuccessful = value;
}

bool Player::TEST_CapturePokemon(int index, Pokemon& pkmn)
{
	auto itr = std::find_if(items.begin(), items.end(), [&](std::pair<std::shared_ptr<Item>, int> item) {return index == item.first->GetID(); });
	if (itr != items.end()) {
		std::shared_ptr<Ball> ball = std::dynamic_pointer_cast<Ball, Item>(itr->first);
		if (ball != nullptr) {
			std::mt19937 rng(std::random_device{}());
			std::uniform_int_distribution<int> dist(0, 100);
			if (dist(rng) < ball->GetProbability()) {
				pokemon.push_back(pkmn);
				selectedPokemon = pokemon.begin();
				return true;
			}
		}
	}
	return false;
}

void Player::TEST_PickUpItem(std::shared_ptr<Item> item)
{
	auto itr = std::find_if(items.begin(), items.end(), [&](std::pair<std::shared_ptr<Item>, int> i) {return item->GetID() == i.first->GetID(); });
	if (itr != items.end()) {
		itr->second++;
	}
	else {
		items.insert(std::pair<std::shared_ptr<Item>, int>(item, 1));
	}
}

void Player::TEST_UseItem(int indexItem, int indexPkmn)
{
	auto itr = std::find_if(items.begin(), items.end(), [&](std::pair<std::shared_ptr<Item>, int> item) {return indexItem == item.first->GetID(); });
	if (itr != items.end()) {
		std::shared_ptr<Consumable> consumable = std::dynamic_pointer_cast<Consumable, Item>(itr->first);
		if (consumable != nullptr) {
			consumable->UseItem(pokemon.at(indexPkmn));
			if (itr->second > 1) {
				itr->second--;
			}
			else {
				items.erase(itr);
			}
		}
		else {
			printf("Can't use that item now!\n");
		}
	}
}

bool Player::TEST_CanUseItem(int index)
{
	auto itr = std::find_if(items.begin(), items.end(), [&](std::pair<std::shared_ptr<Item>, int> item) {return index == item.first->GetID(); });
	if (itr != items.end()) {
		return (std::dynamic_pointer_cast<Consumable, Item>(itr->first) != nullptr);
	}
	return false;
}

bool Player::IsUsingSpecial() const
{
	return bIsUsingSpecial;
}

bool Player::SpecialIsSuccessful() const
{
	return bSpecialIsSuccessful;
}

Pokemon::Type Player::GetUsedPokemonType()
{
	Pokemon::Type output = usedPkmnType;
	usedPkmnType = Pokemon::Type::Normal;
	return output;
}

std::map<std::shared_ptr<Item>, int> Player::GetItemList() const
{
	return items;
}
