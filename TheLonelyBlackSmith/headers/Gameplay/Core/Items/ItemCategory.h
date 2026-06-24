#pragma once
#include "ItemType.h"
#include "Gameplay/Core/RessourceType.h"
#include <iostream>

/* Item Category */
struct ItemCategory
{
	/* Item type */
	ItemType type_;

	/* Ressource type */
	RessourceType ressource_;

	/* Constructor */
	ItemCategory(ItemType type = ItemType::NONE, RessourceType ressource = RessourceType::NONE) : type_(type), ressource_(ressource) {}

	/* < operator */
	bool operator<(const ItemCategory& other) const {
		if (type_ != other.type_) return type_ < other.type_;
		return ressource_ < other.ressource_;
	}

	/* == operator */
	bool operator==(const ItemCategory& other) const {
		return type_ == other.type_ && ressource_ == other.ressource_;
	}

	/* << operator */
	friend std::ostream& operator<<(std::ostream& os, const ItemCategory& cat) {
		os << "ItemCategory(type=" << static_cast<int>(cat.type_)
			<< ", ressource=" << static_cast<int>(cat.ressource_) << ")";
		return os;
	}
};