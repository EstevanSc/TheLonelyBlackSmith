#pragma once
#include <list>
#include <map>
#include <vector>
#include <Gameplay/Core/Items/ItemCategory.h>
#include <Gameplay/Core/StructureType.h>
#include <Gameplay/Core/Recipe.h>
#include <Gameplay/Core/RessourceType.h>
#include <Gameplay/Systems/CraftSystem.h>
#include <Gameplay/Systems/GatherRessourcesSystem.h>
#include <Config/Config.h>

class Game;
class Player;

class BotSystem {
private:
	/* priorities for item craft */
	std::list<ItemCategory> itemCraftPriority_;
	/* priorities for structure craft */
	std::list<StructureType> structureCraftPriority_;

public:
	/* Constructor */
	BotSystem();
	BotSystem(Config& config);

	/* Method to play a turn for the bot */
	void playTurn(Game& game, Player& player, CraftSystem& craftSystem, GatherRessourcesSystem& gatherSystem);

private:
	/* Method to try to craft the best item or structure possible */ 
	bool tryToCraft(Game& game, Player& player, CraftSystem& craftSystem);
	
	/* Method to get the most needed ressource for crafting */
	RessourceType getMostNeededRessource(Player& player, CraftSystem& craftSystem, GatherRessourcesSystem& gatherSystem);

	/* Method to build the items priorities*/
	void buildItemCraftPriority(const std::map<ItemCategory, Recipe> itemRecipes);

	/* Method to build the structures priorities*/
	void buildStructureCraftPriority(const std::map<StructureType, Recipe> structureRecipes);
};