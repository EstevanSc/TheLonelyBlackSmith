#pragma once
#include <map>
#include <Gameplay/Core/Items/ItemCategory.h>
#include <Gameplay/Core/Items/Item.h>
#include <Config/Config.h>

/* Component to handle items Inventory behaviour */
class ItemsManager
{
public:
	/* Items Inventory */
	std::map< ItemCategory, Item> items_;

public:
	/* Constructor */
	ItemsManager(std::map< ItemCategory, Item> initialItems = {});

	/* Destructor */
	~ItemsManager();

	/* Method to add an item */
	void addItem(const Item& item);

	/* Method to get the higher item of a type*/
	Item* getHighestItemOfType(ItemType type) const;

	/* Method to check if an ItemCategory is owned */
	bool hasItem(const ItemCategory& category);

	/* Method to show owned tools list */
	void showOwnedTools() const;
	
};

