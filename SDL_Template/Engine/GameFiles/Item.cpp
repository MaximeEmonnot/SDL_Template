#include "Item.h"

Item::Item(const std::string& name, int ID)
	:
	name(name),
	ID(ID)
{
}

int Item::GetID() const
{
	return ID;
}

std::string Item::GetName() const
{
	return name;
}

bool Item::operator==(const Item& rhs) const
{
	return name == rhs.name;
}

bool Item::operator<(const Item& rhs) const
{
	return ID < rhs.ID;
}
