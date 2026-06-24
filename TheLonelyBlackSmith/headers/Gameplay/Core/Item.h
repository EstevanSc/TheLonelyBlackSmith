#pragma once
#include "ItemType.h"
#include "RessourceType.h"
#include <map>

/* Item struct for crafting */
struct Item
{
	ItemType type;
	RessourceType ressource;
	std::map<RessourceType, int> recipe;
};