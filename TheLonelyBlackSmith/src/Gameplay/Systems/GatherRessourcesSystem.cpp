#include "Gameplay/Systems/GatherRessourcesSystem.h"
#include <Gameplay/GameLoop/Game.h>
#include <stdexcept>
#include <iostream>

GatherRessourcesSystem::GatherRessourcesSystem()
{
	Config config;
	lootRanges_ = config.LOOT_RANGES;
	gatheringTurns_ = config.TURNS_TO_COLLECT;
	ressourceNames_ = config.RESSOURCE_NAMES;
}

GatherRessourcesSystem::GatherRessourcesSystem(Config& config) : lootRanges_(config.LOOT_RANGES), gatheringTurns_(config.TURNS_TO_COLLECT), ressourceNames_(config.RESSOURCE_NAMES)
{
}

bool GatherRessourcesSystem::gatherRessources(Game& game, Player& player, RessourceType ressourceType)
{
	ItemsManager* itemsManager = player.getItemsManager();
	RessourcesManager* ressourcesManager = player.getRessourcesManager();
	if (ressourcesManager == nullptr || itemsManager == nullptr) {
		throw std::runtime_error("GatherRessourcesSystem::gatherRessources, Player does not have a RessourcesManager or ItemsManager.");
		return false;
	}
	std::list<ItemCategory> higestOwnedItems;
	for (int i =0; i < static_cast<int>(ItemType::COUNT); ++i) {
		ItemType itemType = static_cast<ItemType>(i);
		Item* highestItem = itemsManager->getHighestItemOfType(itemType);
		if (highestItem != nullptr) {
			higestOwnedItems.push_back(highestItem->category_);
		}
	}
	ItemCategory defaultItemCategory = { ItemType::NONE, RessourceType::NONE };
	higestOwnedItems.push_back(defaultItemCategory);

	LootRange bestLootRange = { 0, 0 };
	for (const auto& itemCategory : higestOwnedItems) {
		auto it = lootRanges_.find({ itemCategory, ressourceType });
		if (it != lootRanges_.end()) {
			const LootRange& currentLootRange = it->second;
			if (bestLootRange < currentLootRange) {
				bestLootRange = currentLootRange;
			}
		}
	}

	int gatheredAmount = bestLootRange.getRandomLoot();
	if (bestLootRange.min_ == 0 && bestLootRange.max_ == 0) {
		std::cout << "Aucun outil adapté trouvé pour collecter "<< ressourceNames_.at(ressourceType) << "." << std::endl;
		return false;
	}

	int turnsNeeded = gatheringTurns_[ressourceType];
	if (turnsNeeded <= 0) {
		throw std::runtime_error("GatherRessourcesSystem::gatherRessources, Invalid turns needed for gathering.");
		return false;
	}
	if (!game.canIncreaseTurn(turnsNeeded)) {
		std::cout << "Vous n'avez pas assez de tours restants pour collecter " << ressourceNames_.at(ressourceType) << ", celà prendrait " << turnsNeeded << " tours." << std::endl;
		return false;
	}

	ressourcesManager->addRessource(ressourceType, gatheredAmount);
	if (ressourceNames_.find(ressourceType) != ressourceNames_.end()) {
		std::cout << gatheredAmount << " de " << ressourceNames_[ressourceType] << " collecté en " << turnsNeeded << " tours." << std::endl;
	}
	else {
		std::cout << gatheredAmount << " d'un type de ressource inconnu collecté en " << turnsNeeded << " tours." << std::endl;
	}

	game.increaseTurn(turnsNeeded);
	return true;
}

bool GatherRessourcesSystem::canGatherRessource(Player& player, RessourceType ressourceType) const
{
	ItemsManager* itemsManager = player.getItemsManager();
	RessourcesManager* ressourcesManager = player.getRessourcesManager();
	if (ressourcesManager == nullptr || itemsManager == nullptr) {
		throw std::runtime_error("GatherRessourcesSystem::gatherRessources, Player does not have a RessourcesManager or ItemsManager.");
		return false;
	}
	std::list<ItemCategory> higestOwnedItems;
	for (int i = 0; i < static_cast<int>(ItemType::COUNT); ++i) {
		ItemType itemType = static_cast<ItemType>(i);
		Item* highestItem = itemsManager->getHighestItemOfType(itemType);
		if (highestItem != nullptr) {
			higestOwnedItems.push_back(highestItem->category_);
		}
	}
	LootRange bestLootRange = { 0, 0 };
	for (const auto& itemCategory : higestOwnedItems) {
		auto it = lootRanges_.find({ itemCategory, ressourceType });
		if (it != lootRanges_.end()) {
			const LootRange& currentLootRange = it->second;
			if (bestLootRange < currentLootRange) {
				bestLootRange = currentLootRange;
			}
		}
	}

	if (bestLootRange.min_ == 0 && bestLootRange.max_ == 0) {
		return false;
	}
	return true;
}

void GatherRessourcesSystem::showGatherOptions(Player& player) const
{
	for (int i = 1; i < static_cast<int>(RessourceType::COUNT); ++i) {
		RessourceType ressourceType = static_cast<RessourceType>(i);
		if (ressourceNames_.find(ressourceType) != ressourceNames_.end()) {
			std::cout << i << ". " << ressourceNames_.at(ressourceType);
		}
		else {
			std::cout << i << ". unknown";
		}
		if (i < static_cast<int>(RessourceType::COUNT) - 1) {
			std::cout << " | ";
		}
	}
	std::cout << std::endl;
}

int GatherRessourcesSystem::getNumberOfGatherOptions() const
{
	int numberOfOptions = 0;
	for (int i = 0; i < static_cast<int>(RessourceType::COUNT); ++i) {
		RessourceType ressourceType = static_cast<RessourceType>(i);
		if (ressourceNames_.find(ressourceType) != ressourceNames_.end()) {
			numberOfOptions++;
		}
	}
	return numberOfOptions;
}

int GatherRessourcesSystem::getMinimumTurnsToGather() const
{
	int minTurns = std::numeric_limits<int>::max();
	for (const auto& pair : gatheringTurns_) {
		if (pair.second < minTurns) {
			minTurns = pair.second;
		}
	}
	return minTurns == std::numeric_limits<int>::max() ? 0 : minTurns;
}
