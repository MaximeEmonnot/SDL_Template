#include "Player.h"
#include <random>

Player::Player(Maths::IRect rect, const std::string& animFile)
	:
	Character(rect),
	pKbd(CoreSystem::Keyboard::GetInstance())
{
	JSONParser::Reader jsonParse;
	jsonParse.ReadFile(animFile);

	mSprite.InitSurface(jsonParse.GetValueOf("filename").GetString());

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

	miCurSequence = (int)AnimationList::StandingRight;
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

	//Init Pokemon list
	if (jsonReaderSF.IsValueAvailable("Pokemons")) {
		auto& pokemonValue = jsonReaderSF.GetValueOf("Pokemons");
		for (auto itr = pokemonValue.MemberBegin(); itr != pokemonValue.MemberEnd(); ++itr) {
			Pokemon newPkmn;

			newPkmn.name = itr->name.GetString();
			newPkmn.hp = itr->value.GetArray()[0].GetInt();
			newPkmn.currentMaxHP = itr->value.GetArray()[1].GetInt();
			newPkmn.att = itr->value.GetArray()[2].GetInt();
			newPkmn.def = itr->value.GetArray()[3].GetInt();
			newPkmn.lvl = itr->value.GetArray()[4].GetInt();
			newPkmn.id = itr->value.GetArray()[5].GetInt();
			newPkmn.pGfx = GraphicsEngine::Graphics::GetInstance();
			switch (newPkmn.id)
			{
			case 1:
				newPkmn.type = Pokemon::Type::Grass;
				newPkmn.sprite.InitSurface("Images/bulbasaur.png");
				break;
			case 2:
				newPkmn.type = Pokemon::Type::Fire;
				newPkmn.sprite.InitSurface("Images/charmander.png");
				break;
			case 3:
				newPkmn.type = Pokemon::Type::Water;
				newPkmn.sprite.InitSurface("Images/squirttle.png");
				break;
			default:
				break;
			}

			AddPokemon(newPkmn);
		}
	}

	//Init Pokemons abilities
	for (auto& pkmn : pokemon) {
		auto& pokemonValue = jsonReaderSF.GetValueOf(pkmn.name);
		for (auto itr = pokemonValue.MemberBegin(); itr != pokemonValue.MemberEnd(); ++itr) {
			pkmn.LoadAbility(Pokemon::Ability(itr->name.GetString(), itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt(), pkmn.type));
		}
	}

}

void Player::SaveJSON()
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

	//Save selected pkmn
	jsonWriter.AddObjectMember("Pokemons", selectedPokemon->name , selectedPokemon->hp, selectedPokemon->currentMaxHP, selectedPokemon->att, selectedPokemon->def, selectedPokemon->lvl, selectedPokemon->id);
	for (auto& ability : selectedPokemon->GetAbilities()) {
		jsonWriter.AddObjectMember(selectedPokemon->name, ability.GetName(), ability.GetPower(), ability.GetMaxPP());
	}

	//Save Pokemons
	for (auto& pkmn : pokemon) {
		if (pkmn != *selectedPokemon) {
			jsonWriter.AddObjectMember("Pokemons", pkmn.GetName(), pkmn.hp, pkmn.currentMaxHP, pkmn.att, pkmn.def, pkmn.lvl, pkmn.id);
			for (auto& ability : pkmn.GetAbilities()) {
				jsonWriter.AddObjectMember(pkmn.name, ability.GetName(), ability.GetPower(), ability.GetMaxPP());
			}
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

	//Movement
	//mRect += dir * (int)speed;
	
	//Animation
	if (dir != Maths::IVec2D(0,0)) {
		if (dir.y != 0) {
			if (dir.y > 0) {
				miCurSequence = (int)AnimationList::WalkingDown;
				lookingDirection = Maths::IVec2D(0, 1);
			}
			else {
				miCurSequence = (int)AnimationList::WalkingUp;
				lookingDirection = Maths::IVec2D(0, -1);
			}
		}
		else {
			if (dir.x > 0) {
				miCurSequence = (int)AnimationList::WalkingRight;
				lookingDirection = Maths::IVec2D(1, 0);
			}
			else {
				miCurSequence = (int)AnimationList::WalkingLeft;
				lookingDirection = Maths::IVec2D(-1, 0);
			}
		}
	}
	else {
		if (velocity != Maths::IVec2D(0, 0)) {
			if (velocity.y != 0) {
				if (velocity.y > 0) {
					miCurSequence = (int)AnimationList::StandingDown;
				}
				else {
					miCurSequence = (int)AnimationList::StandingUp;
				}
			}
			else {
				if (velocity.x > 0) {
					miCurSequence = (int)AnimationList::StandingRight;
				}
				else {
					miCurSequence = (int)AnimationList::StandingLeft;
				}
			}
		}
	}

	//Current Velocity
	velocity = dir;
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

std::map<std::shared_ptr<Item>, int> Player::GetItemList() const
{
	return items;
}