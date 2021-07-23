#include "Player.h"

Player::Player(Maths::IRect rect, const std::string& animFile)
	:
	Character(rect),
	//pGrid(Grid::GetInstance()),
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
	JSONParser::Reader jsonReader;
	jsonReader.ReadFile("json/saveFile.json");

	//Init consumables
	if (jsonReader.IsValueAvailable("Consumables")) {
		auto& consumableValue = jsonReader.GetValueOf("Consumables");
		for (auto itr = consumableValue.MemberBegin(); itr != consumableValue.MemberEnd(); ++itr) {
			items.insert(std::pair<std::shared_ptr<Item>, int>(std::make_shared<Consumable>(itr->name.GetString(), itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt()), itr->value.GetArray()[2].GetInt()));
		}
	}

	//Init balls
	if (jsonReader.IsValueAvailable("Balls")) {
		auto& ballValue = jsonReader.GetValueOf("Balls");
		for (auto itr = ballValue.MemberBegin(); itr != ballValue.MemberEnd(); ++itr) {
			items.insert(std::pair<std::shared_ptr<Item>, int>(std::make_shared<Ball>(itr->name.GetString(), itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt()), itr->value.GetArray()[2].GetInt()));
		}
	}

	//Init Pokemon
	auto& pokemonValue = jsonReader.GetValueOf("Pokemon");
	pokemon.hp = pokemonValue.FindMember("HP")->value.GetInt();
	pokemon.att = pokemonValue.FindMember("Attack")->value.GetInt();
	pokemon.def = pokemonValue.FindMember("Defense")->value.GetInt();
	pokemon.lvl = pokemonValue.FindMember("Level")->value.GetInt();
	pokemon.id = pokemonValue.FindMember("ID")->value.GetInt();
	pokemon.pGfx = GraphicsEngine::Graphics::GetInstance();
	switch (pokemon.id)
	{
	case 1:
		pokemon.sprite.InitSurface("Images/bulbasaur.png");
		break;
	case 2:
		pokemon.sprite.InitSurface("Images/charmander.png");
		break;
	case 3:
		pokemon.sprite.InitSurface("Images/squirttle.png");
		break;
	default:
		break;
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

	//Save PKMN
	jsonWriter.AddObjectMember("Pokemon", "HP", pokemon.hp);
	jsonWriter.AddObjectMember("Pokemon", "Attack", pokemon.att);
	jsonWriter.AddObjectMember("Pokemon", "Defense", pokemon.def);
	jsonWriter.AddObjectMember("Pokemon", "Level", pokemon.lvl);
	jsonWriter.AddObjectMember("Pokemon", "ID", pokemon.id);

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

void Player::SetPokemon(Pokemon& pkmn)
{
	pokemon = pkmn;
}

Pokemon& Player::GetPokemon()
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
		pokemon.DrawFrontSprite(Maths::IRect(250, 250, 128, 128));
	}
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

void Player::TEST_UseItem(int index)
{
	auto itr = std::find_if(items.begin(), items.end(), [&](std::pair<std::shared_ptr<Item>, int> item) {return index == item.first->GetID(); });
	if (itr != items.end()) {
		std::shared_ptr<Consumable> consumable = std::dynamic_pointer_cast<Consumable, Item>(itr->first);
		if (consumable != nullptr) {
			consumable->UseItem(pokemon);
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

std::map<std::shared_ptr<Item>, int> Player::GetItemList() const
{
	return items;
}
