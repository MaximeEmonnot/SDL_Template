#pragma once
#include <string>
#include "Pokemon.h"

class Item {
public:
	class Hash {
	public:
		size_t operator()(const Item& i) const {
			std::hash<std::string> hasher;
			return (hasher(i.name));
		}
	};
public:
	Item(const std::string& name, int ID);

	virtual void UseItem(Pokemon& pkmn) = 0;
	
	int GetID() const;
	std::string GetName() const;

	bool operator== (const Item& rhs) const;
	bool operator< (const Item& rhs) const;

protected:
	std::string name;
	int ID;
};