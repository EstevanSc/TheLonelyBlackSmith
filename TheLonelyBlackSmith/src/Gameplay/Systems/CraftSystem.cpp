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

	if (itemsManager->hasItem(itemCategory)) {
		std::cout << "Impossible de fabriquer : " << name << " car vous possédez déjà cet objet." << std::endl;
		return false;
	}

	
	if (!hasPrerequisitesForCraft(player, recipe)) {
		std::cout << "Impossible de fabriquer : " << name << " car vous ne possédez pas les prérequis." << std::endl;
		return false;
	}
	if (!hasRessourcesForCraft(player, recipe)) {
		std::cout << "Impossible de fabriquer : " << name << " car vous n'avez pas assez de ressources." << std::endl;
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
		std::cout << "Impossible de fabriquer la structure : " << name << " car vous ne possédez pas les prérequis." << std::endl;
		return false;
	}
	if (!hasRessourcesForCraft(player, recipe)) {
		std::cout << "Impossible de fabriquer la structure : " << name << " car vous n'avez pas assez de ressources." << std::endl;
		return false;
	}

	finishCraft(game, player, *ressourcesManager, recipe, name);
	if (constructedStructures_.find(structureType) != constructedStructures_.end()) {
		constructedStructures_[structureType]++;
	}
	else {
		constructedStructures_[structureType] = 1;
	}
	return true;
}

void CraftSystem::showCraftList(Player& player) const
{
	if (!checkPlayerComponents(player)) {
		throw std::runtime_error("CraftSystem::showCraftList() : Player's components are not initialized.");
		return;
	}
	ItemsManager* itemsManager = player.getItemsManager();
	int index = 1;
	for (const auto& [itemCategory, recipe] : itemRecipes_) {
		if (!isValidItemCategory(itemCategory)) {
			continue;
		}
		std::string name = itemNames_.at(itemCategory);
		bool hasPrerequisites = hasPrerequisitesForCraft(player, recipe);
		bool hasRessources = hasRessourcesForCraft(player, recipe);
		Recipe recipe = itemRecipes_.at(itemCategory);
		int numberOfTurns = recipe.turns_;
		std::cout << index << ". Objet : " << name << " (Tours nécessaires : " << numberOfTurns << ")" << std::endl;
		if (itemsManager->hasItem(itemCategory)) {
			std::cout << "  Vous possédez déjà cet objet." << std::endl;
		}
		else {
			showRecipe(recipe, hasPrerequisites, hasRessources);
		}
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
		int numberOfTurns = recipe.turns_;
		std::cout << index << ". Structure : " << name << " (Tours nécessaires : " << numberOfTurns << ")" << std::endl;
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

void CraftSystem::showConstructedStructures() const
{
	std::cout << "\nStructures construites :" << std::endl;
	for (const auto& [structureType, count] : constructedStructures_) {
		if (!isValidStructureType(structureType)) {
			continue;
		}
		std::string name = structureNames_.at(structureType);
		std::cout << "Structure : " << name << ", Quantité : " << count << std::endl;
	}
	if (constructedStructures_.empty()) {
		std::cout << "Aucune structure n'a encore été construite." << std::endl;
	}
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
	std::cout << name << " fabriqué(e) ! Score augmenté de " << scoreToAdd << ". Tours augmentés de " << recipe.turns_ << "." << std::endl;
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
		std::cout << "  Prérequis : Aucun" << std::endl;
	} else {
		std::cout << "  Prérequis : ";
		for (const auto& prerequisite : recipe.prerequisites_) {
			if (!itemNames_.count(prerequisite)) {
				throw std::runtime_error("CraftSystem::showRecipe() : Name for the given prerequisite item category not found.");
			}
			std::string prerequisiteName = itemNames_.at(prerequisite);
			std::cout << prerequisiteName << " ";
		}
		std::cout << std::endl;
	}
	if (!hasPrerequisites) {
		std::cout << "  Vous n'avez pas les prérequis pour fabriquer ceci." << std::endl;
	}
	// Show ressources
	if (recipe.ressources_.empty()) {
		std::cout << "  Coût en ressources : Aucun" << std::endl;
	} else {
		std::cout << "  Coût en ressources : ";
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
		std::cout << "  Vous n'avez pas assez de ressources pour fabriquer ceci." << std::endl;
	}
}
