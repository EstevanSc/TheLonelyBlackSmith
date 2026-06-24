#pragma once
#include <map>
#include <Config/Config.h>
#include <Gameplay/Entities/Player.h>

class Game;

/* System handling crafts */
class CraftSystem
{
private:
	/* recipes for items crafts */
	std::map<ItemCategory, Recipe> itemRecipes_;

	/* recipes for structure crafts */
	std::map<StructureType, Recipe> structureRecipes_;

	/* name of items */
	std::map<ItemCategory, std::string> itemNames_;

	/* name of ressources */
	std::map<RessourceType, std::string> ressourceNames_;

	/* name of structures */
	std::map<StructureType, std::string> structureNames_;

	/* constructed structures */
	std::map<StructureType, int> constructedStructures_;

public:
	/* Constructors */
	CraftSystem();
	CraftSystem(Config& config);

	/* Method to check if a player can craft an item */
	bool canCraftItem(Player& player, const ItemCategory& itemCategory) const;

	/* Method to check if a player fit the craft a structure */
	bool canCraftStructure(Player& player, StructureType structureType) const;

	/* Method to craft an item */
	bool craftItem(Game& game, Player& player, const ItemCategory& itemCategory);

	/* Method to craft a structure */
	bool craftStructure(Game& game, Player& player, StructureType structureType);

	/* Method to show the craft list with possibility and craft */
	void showCraftList(Player& player) const;

	/* Getter for the number of crafts */
	int getNumberOfCrafts() const;

	/* Method to craft using a string choice */
	bool craftByChoice(Game& game, Player& player, const std::string& choice);

	/* Method to show constructed structures */
	void showConstructedStructures() const;

	/* Method to get the minimum turn to craft something */
	int getMinimumTurnToCraft(Player& player) const;

private:
	// helper methods

	/* Method to check if a player fit requirements for a craft */
	bool hasPrerequisitesForCraft(Player& player, const Recipe recipe) const;

	/* Method to check if a player has the ressources to craft an item */
	bool hasRessourcesForCraft(Player& player, const Recipe recipe) const;

	/* Method to check player components */
	bool checkPlayerComponents(Player& player) const;

	/* Method to finish a craft */
	void finishCraft(Game& game, Player& player, RessourcesManager& ressourcesManager, const Recipe recipe, std::string name);

	/* Method to check the validity of an item */
	bool isValidItemCategory(const ItemCategory& itemCategory) const;

	/* Method to check the validity of a structure */ 
	bool isValidStructureType(StructureType structureType) const;

	/* Method to show a recipe in the list */
	void showRecipe(const Recipe& recipe, bool hasPrerequisites, bool hasRessources) const;
};

