#pragma once
#include "ItemType.h"
#include <map>
#include <string>
#include "ItemCategory.h"
#include "Gameplay/Core/RessourceType.h"

/* Item struct for crafting */
struct Item
{
	/* name of the item */
	std::string name_;

	/* category of the item */ 
	ItemCategory category_;

	/* recipe for crafting the item */
	std::map<RessourceType, int> recipe_;

	/* prerequise for the item craft */
	std::map<ItemType, ItemType> prerequise_;
};