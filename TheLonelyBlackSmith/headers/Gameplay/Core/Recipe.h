#pragma once
#include <map>
#include "RessourceType.h"
#include <list>
#include "Items/ItemCategory.h"

/* Recipe for crafting */
struct Recipe
{
	/* Ressources needed for craft */
	std::map<RessourceType, int> ressources_;

	/* Prerequisites for craft */
	std::list<ItemCategory> prerequisites_;

	/* Score given by this recipe */
	int score_;

	/* Turns to use this recipe */
	int turns_;
};