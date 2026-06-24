#include "Gameplay/Components/ItemsManager.h"

ItemsManager::ItemsManager(std::map<ItemCategory, Item> initialItems)
{
	items_ = initialItems;
}

ItemsManager::~ItemsManager()
{
}
