#pragma once
#include <stdlib.h>

/* Loot range for the game ressources */ 
struct LootRange
{
	/* Minimum amount of loot */
	int min_;
	/* Maximum amount of loot */
	int max_;

	/* method to get a random loot amount */
	int getRandomLoot() const {
		return min_ + (rand() % (max_ - min_ + 1));
	}
};