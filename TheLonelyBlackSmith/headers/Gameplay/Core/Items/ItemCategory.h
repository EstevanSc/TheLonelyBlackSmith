#pragma once
#include "ItemType.h"

/* Item Category */
struct ItemCategory
{
	/* Item type */
	ItemType type_;

	/* Ressource type */
	RessourceType ressource_;

	/* Constructor */
	ItemCategory(ItemType t, RessourceType r) : type_(t), ressource_(r) {}

	/* < operator */
	bool operator<(const ItemCategory& other) const {
		if (type_ != other.type_) return type_ < other.type_;
		return ressource_ < other.ressource_;
	}

	/* == operator */
	bool operator==(const ItemCategory& other) const {
		return type_ == other.type_ && ressource_ == other.ressource_;
	}
};