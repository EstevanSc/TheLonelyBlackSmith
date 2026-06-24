#include "Gameplay/Components/ItemsManager.h"
#include <stdexcept>

ItemsManager::ItemsManager(std::map<ItemCategory, Item> initialItems)
{
	items_ = initialItems;
}

ItemsManager::~ItemsManager()
{
}

void ItemsManager::addItem(const Item& item)
{
	if (items_.find(item.category_) != items_.end()) {
		throw std::runtime_error("Item already exists in the inventory.");
	}
	else {
		items_[item.category_] = item;
	}
}

Item* ItemsManager::getHighestItemOfType(ItemType type) const
{
	std::list<Item> itemsOfType;
	for (const auto& pair : items_) {
		if (pair.first.type_ == type) {
			itemsOfType.push_back(pair.second);
		}
	}
	// order the list by ressource type (WOOD < STONE < IRON)
	itemsOfType.sort([](const Item& a, const Item& b) {
		return a.category_.ressource_ < b.category_.ressource_;
		});
	// return the last item in the list (the highest)
	if (!itemsOfType.empty()) {
		return &itemsOfType.back();
	}
	else {
		return nullptr;
	}
}

bool ItemsManager::hasItem(const ItemCategory& category)
{
	for (const auto& pair : items_) {
		if (pair.first == category) {
			return true;
		}
	}
	return false;
}
