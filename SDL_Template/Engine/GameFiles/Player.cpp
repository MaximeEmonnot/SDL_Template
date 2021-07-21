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

void Player::InitFromJSON()
{
	JSONParser::Reader jsonReader;
	jsonReader.ReadFile("json/saveFile.json");

	//Init inventory
	auto& inventoryValue = jsonReader.GetValueOf("Inventory");
	for (auto itr = inventoryValue.MemberBegin(); itr != inventoryValue.MemberEnd(); ++itr) {
		items.insert(std::pair<Maths::IVec2D, std::string>(Maths::IVec2D(itr->value.GetArray()[0].GetInt(), itr->value.GetArray()[1].GetInt()), itr->name.GetString()));
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
		jsonWriter.AddObjectMember("Inventory", entry.second, entry.first.x, entry.first.y);
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

void Player::TEST_PickUpItem(const std::pair<Maths::IVec2D, std::string>& item)
{
	items.insert(item);
}

void Player::TEST_UseItem(int index)
{
	auto itr = items.begin();
	for (int i = 0; i < index; i++) {
		itr++;
	}
	items.erase(itr);
}

std::unordered_map<Maths::IVec2D, std::string, Maths::IVec2D::Hash> Player::GetItemList() const
{
	return items;
}
