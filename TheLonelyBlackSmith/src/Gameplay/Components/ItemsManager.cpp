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
    Item* highestItem = nullptr;

    for (const auto& pair : items_) {
        const Item& currentItem = pair.second;

        if (currentItem.category_.type_ == type) {
            if (highestItem == nullptr ||
                currentItem.category_.ressource_ > highestItem->category_.ressource_) {
                highestItem = const_cast<Item*>(&currentItem);
            }
        }
    }

    if (highestItem != nullptr) {
        return highestItem;
    }

    Item defaultNoneItem;
    defaultNoneItem.category_ = ItemCategory(type, RessourceType::NONE);
    return &defaultNoneItem;
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

void ItemsManager::showOwnedTools() const
{
    std::cout << "\nOwned Tools:" << std::endl;
    for (const auto& pair : items_) {
        const Item& item = pair.second;
        std::cout << "- " << item.name_ << std::endl;
	}
    if (items_.empty()) {
        std::cout << "No tools owned." << std::endl;
	}
}
