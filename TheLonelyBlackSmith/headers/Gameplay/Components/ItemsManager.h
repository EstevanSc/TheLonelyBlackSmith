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
};

