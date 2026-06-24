#pragma once
#include <Config/Config.h>
#include <Gameplay/Entities/Player.h>

class Game;

/* System to handle gathering ressources */
class GatherRessourcesSystem
{
private:
	/* loot ranges for each ressource type depending of tool */
	std::map<std::pair<ItemCategory, RessourceType>, LootRange> lootRanges_;

	/* turns needed to gather a ressource type */
	std::map<RessourceType, int> gatheringTurns_;

	/* ressources names */
	std::map<RessourceType, std::string> ressourceNames_;

public:
	/* Constructor */ 
	GatherRessourcesSystem();
	GatherRessourcesSystem(Config& config);

	/* Method to gather ressources, return if it's a success or not */
	bool gatherRessources(Game& game, Player& player, RessourceType ressourceType); 

	/* Method to show the gahter options */
	void showGatherOptions(Player& player) const;

	/* Method to get the gather possibility number */
	int getNumberOfGatherOptions() const;
};

