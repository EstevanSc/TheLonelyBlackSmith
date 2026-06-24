#include "Gameplay/Systems/CraftSystem.h"
#include <stdexcept>
#include <iostream>
#include <Gameplay/GameLoop/Game.h>

CraftSystem::CraftSystem()
{
	Config config;
	itemRecipes_ = config.ITEM_RECIPES;
	structureRecipes_ = config.STRUCTURE_RECIPES;
	itemNames_ = config.ITEM_NAMES;
	structureNames_ = config.STRUCTURE_NAMES;
	ressourceNames_ = config.RESSOURCE_NAMES;
}

CraftSystem::CraftSystem(Config& config) : itemRecipes_(config.ITEM_RECIPES), structureRecipes_(config.STRUCTURE_RECIPES),
itemNames_(config.ITEM_NAMES), structureNames_(config.STRUCTURE_NAMES), ressourceNames_(config.RESSOURCE_NAMES)
{
}

bool CraftSystem::canCraftItem(Player& player, const ItemCategory& itemCategory) const
{
	if (itemRecipes_.find(itemCategory) == itemRecipes_.end()) {
		throw std::runtime_error("CraftSystem::canCraftItem() : Recipe for the given item category not found.");
		return false;
	}
	Recipe recipe = itemRecipes_.at(itemCategory);
	return hasPrerequisitesForCraft(player, recipe) && hasRessourcesForCraft(player, recipe);
}

bool CraftSystem::canCraftStructure(Player& player, StructureType structureType) const
{
	if (structureRecipes_.find(structureType) == structureRecipes_.end()) {
		throw std::runtime_error("CraftSystem::canCraftStructure() : Recipe for the given structure type not found.");
		return false;
	}
	Recipe recipe = structureRecipes_.at(structureType);
	return hasPrerequisitesForCraft(player, recipe) && hasRessourcesForCraft(player, recipe);
}

bool CraftSystem::craftItem(Game& game, Player& player, const ItemCategory& itemCategory)
{
	if (!checkPlayerComponents(player)) {
		throw std::runtime_error("CraftSystem::craftStructure() : Player's components are not initialized.");
		return false;
	}
	ItemsManager* itemsManager = player.getItemsManager();
	RessourcesManager* ressourcesManager = player.getRessourcesManager();

	if (!isValidItemCategory(itemCategory)) {
		throw std::runtime_error("CraftSystem::craftItem() : Invalid item category.");
		return false;
	}

	Recipe recipe = itemRecipes_.at(itemCategory);
	std::string name = itemNames_.at(itemCategory);
	if (!hasPrerequisitesForCraft(player, recipe)) {
		std::cout << "Can't craft the item : " << name << " because you don't have the prerequisites" << std::endl;
		return false;
	}
	if (!hasRessourcesForCraft(player, recipe)) {
		std::cout << "Can't craft the item : " << name << " because you don't have enough ressources" << std::endl;
		return false;
	}

	finishCraft(game, player, *ressourcesManager, recipe, name);
	Item newItem = { name, itemCategory };
	itemsManager->addItem(newItem);
	return true;
}

bool CraftSystem::craftStructure(Game& game, Player& player, StructureType structureType)
{
	if (!checkPlayerComponents(player)) {
		throw std::runtime_error("CraftSystem::craftStructure() : Player's components are not initialized.");
		return false;
	}
	ItemsManager* itemsManager = player.getItemsManager();
	RessourcesManager* ressourcesManager = player.getRessourcesManager();

	if (!isValidStructureType(structureType)) {
		throw std::runtime_error("CraftSystem::craftStructure() : Invalid structure type.");
		return false;
	}

	Recipe recipe = structureRecipes_.at(structureType);
	std::string name = structureNames_.at(structureType);
	if (!hasPrerequisitesForCraft(player, recipe)) {
		std::cout << "Can't craft the structure : " << name << " because you don't have the prerequisites" << std::endl;
		return false;
	}
	if (!hasRessourcesForCraft(player, recipe)) {
		std::cout << "Can't craft the structure : " << name << " because you don't have enough ressources" << std::endl;
		return false;
	}

	for (const auto& ressource : recipe.ressources_) {
		ressourcesManager->removeRessource(ressource.first, ressource.second);
	}

	finishCraft(game, player, *ressourcesManager, recipe, name);
	return true;
}

void CraftSystem::showCraftList(Player& player) const
{
	int index = 1;
	for (const auto& [itemCategory, recipe] : itemRecipes_) {
		if (!isValidItemCategory(itemCategory)) {
			continue;
		}
		std::string name = itemNames_.at(itemCategory);
		bool hasPrerequisites = hasPrerequisitesForCraft(player, recipe);
		bool hasRessources = hasRessourcesForCraft(player, recipe);
		Recipe recipe = itemRecipes_.at(itemCategory);
		std::cout << index << ". Item: " << name << std::endl;
		showRecipe(recipe, hasPrerequisites, hasRessources);
		std::cout << std::endl;
		index++;
	}
	for (const auto& [structureType, recipe] : structureRecipes_) {
		if (!isValidStructureType(structureType)) {
			continue;
		}
		std::string name = structureNames_.at(structureType);
		bool hasPrerequisites = hasPrerequisitesForCraft(player, recipe);
		bool hasRessources = hasRessourcesForCraft(player, recipe);
		std::cout << index << ". Structure: " << name << std::endl;
		showRecipe(recipe, hasPrerequisites, hasRessources);
		std::cout << std::endl;
		index++;
	}
}

int CraftSystem::getNumberOfCrafts() const
{
	int totalCrafts = itemRecipes_.size() + structureRecipes_.size();
	return totalCrafts;
}

bool CraftSystem::craftByChoice(Game& game, Player& player, const std::string& choice)
{
	int choiceIndex = std::stoi(choice);
	int index = 1;
	for (const auto& [itemCategory, recipe] : itemRecipes_) {
		if (index == choiceIndex) {
			return craftItem(game, player, itemCategory);
		}
		index++;
	}
	for (const auto& [structureType, recipe] : structureRecipes_) {
		if (index == choiceIndex) {
			return craftStructure(game, player, structureType);
		}
		index++;
	}
	return false;
}

bool CraftSystem::hasPrerequisitesForCraft(Player& player, const Recipe recipe) const
{
	if (recipe.prerequisites_.empty()) {
		return true;
	}
	ItemsManager* itemsManager = player.getItemsManager();
	if (!itemsManager) {
		throw std::runtime_error("CraftSystem::hasPrerequisitesForCraft() : Player's ItemsManager is not initialized.");
		return false;
	}

	for (const auto& prerequisite : recipe.prerequisites_) {
		if (!itemsManager->hasItem(prerequisite)) {
			return false;
		}
	}
	return true;
}

bool CraftSystem::hasRessourcesForCraft(Player& player, const Recipe recipe) const
{
	RessourcesManager* ressourcesManager = player.getRessourcesManager();
	if (!ressourcesManager) {
		throw std::runtime_error("CraftSystem::hasRessourcesForCraft() : Player's RessourcesManager is not initialized.");
		return false;
	}
	
	for (const auto& ressource : recipe.ressources_) {
		if (!ressourcesManager->hasEnoughRessources(ressource.first, ressource.second)) {
			return false;
		}
	}
	return true;
}

bool CraftSystem::checkPlayerComponents(Player& player) const
{
	ItemsManager* itemsManager = player.getItemsManager();
	if (!itemsManager) {
		throw std::runtime_error("CraftSystem::craftItem() : Player's ItemsManager is not initialized.");
		return false;
	}

	RessourcesManager* ressourcesManager = player.getRessourcesManager();
	if (!ressourcesManager) {
		throw std::runtime_error("CraftSystem::craftItem() : Player's RessourcesManager is not initialized.");
		return false;
	}
	return true;
}

void CraftSystem::finishCraft(Game& game, Player& player, RessourcesManager& ressourcesManager, const Recipe recipe, std::string name)
{
	for (const auto& ressource : recipe.ressources_) {
		ressourcesManager.removeRessource(ressource.first, ressource.second);
	}
	int scoreToAdd = recipe.score_;
	player.addScore(scoreToAdd);
	game.increaseTurn(recipe.turns_);
	std::cout << "Crafted " << name << "! Score increased by " << scoreToAdd << ". Turns increased by " << recipe.turns_ << "." << std::endl;
}

bool CraftSystem::isValidItemCategory(const ItemCategory& itemCategory) const
{
	if (itemRecipes_.find(itemCategory) == itemRecipes_.end()) {
		throw std::runtime_error("CraftSystem::isValidItemCategory : Recipe for the given item category not found.");
		return false;
	}

	if (itemNames_.find(itemCategory) == itemNames_.end()) {
		throw std::runtime_error("CraftSystem::isValidItemCategory : Name for the given item category not found.");
		return false;
	}
	return true;
}

bool CraftSystem::isValidStructureType(StructureType structureType) const
{
	if (structureRecipes_.find(structureType) == structureRecipes_.end()) {
		throw std::runtime_error("CraftSystem::isValidStructureType : Recipe for the given structure type not found.");
		return false;
	}

	if (structureNames_.find(structureType) == structureNames_.end()) {
		throw std::runtime_error("CraftSystem::isValidStructureType : Name for the given structure type not found.");
		return false;
	}
	return true;
}

void CraftSystem::showRecipe(const Recipe& recipe, bool hasPrerequisites, bool hasRessources) const
{
	// Show prerequisites
	if (recipe.prerequisites_.empty()) {
		std::cout << "  Prerequisites: None" << std::endl;
	} else {
		std::cout << "  Prerequisites: ";
		for (const auto& prerequisite : recipe.prerequisites_) {
			std::cout << prerequisite << " ";
		}
		std::cout << std::endl;
	}
	if (!hasPrerequisites) {
		std::cout << "  You don't have the prerequisites to craft this structure." << std::endl;
	}
	// Show ressources
	if (recipe.ressources_.empty()) {
		std::cout << "  Ressources cost : None" << std::endl;
	} else {
		std::cout << "  Ressources cost : ";
		for (const auto& ressource : recipe.ressources_) {
			if (ressourceNames_.find(ressource.first) == ressourceNames_.end()) {
				throw std::runtime_error("CraftSystem::showRecipe() : Name for the given ressource type not found.");
			}
			std::string ressourceName = ressourceNames_.at(ressource.first);
			std::cout << ressourceName << ": " << ressource.second << " ";
		}
		std::cout << std::endl;
	}
	if (!hasRessources) {
		std::cout << "  You don't have enough ressources to craft this structure." << std::endl;
	}
}
