#pragma once
#include <map>
#include "../Gameplay/Core/RessourceType.h"
#include "../Gameplay/Core/Items/ItemCategory.h"
#include "../Gameplay/Core/Recipe.h"
#include <Gameplay/Core/StructureType.h>
#include <Gameplay/Core/LootRange.h>
#include <string>

struct Config
{
public:
	/* Maximum Number of turns */
	int MAX_TURNS = 100;
	
    /* Map of the initial ressources */
    std::map<RessourceType, int> INITIAL_RESSOURCES = {
        {RessourceType::WOOD, 2},
        {RessourceType::STONE, 1},
        {RessourceType::IRON, 0}
    };

    /* Item crafting recipes */
    const std::map<ItemCategory, Recipe> ITEM_RECIPES = {
        // Pickaxes
        { {ItemType::PICKAXE, RessourceType::WOOD},  {{ {RessourceType::WOOD, 2} }, {}, 10, 1} },
        { {ItemType::PICKAXE, RessourceType::STONE}, {{ {RessourceType::STONE, 3} }, {}, 20, 2} },
        { {ItemType::PICKAXE, RessourceType::IRON},  {{ {RessourceType::IRON, 3} }, {}, 30, 3} },

        // Axes
        { {ItemType::AXE, RessourceType::WOOD},      {{ {RessourceType::WOOD, 3} }, {}, 10, 1} },
        { {ItemType::AXE, RessourceType::STONE},     {{ {RessourceType::STONE, 4} }, {}, 20, 2} },
        { {ItemType::AXE, RessourceType::IRON},      {{ {RessourceType::IRON, 4} }, {}, 30, 3} },

        // Hammers
        { {ItemType::HAMMER, RessourceType::WOOD},   {{ {RessourceType::WOOD, 2}, {RessourceType::STONE, 1} }, {}, 15, 2} },
        
        { {ItemType::HAMMER, RessourceType::STONE},  {{ {RessourceType::STONE, 2}, {RessourceType::IRON, 1} }, 
        {{ItemType::HAMMER, RessourceType::WOOD}}, 25, 3} },
        
        { {ItemType::HAMMER, RessourceType::IRON},   {{ {RessourceType::IRON, 2}, {RessourceType::WOOD, 1} }, 
        {{ItemType::HAMMER, RessourceType::STONE}}, 35, 4} }
    };

    /* Structures recipes*/
    const std::map<StructureType, Recipe> STRUCTURE_RECIPES = {
		// Forge
        { StructureType::FORGE, {{ {RessourceType::STONE, 5}, {RessourceType::IRON, 3} }, 
        {{ItemType::HAMMER, RessourceType::IRON}}, 50, 5} }
	};

	/* Loot ranges for each ressource type depending of tool */
    const std::map<std::pair<ItemCategory, RessourceType>, LootRange> LOOT_RANGES = {
		// no tool
        { {{ItemType::NONE, RessourceType::NONE}, RessourceType::WOOD},  {1, 1} },
        { {{ItemType::NONE, RessourceType::NONE}, RessourceType::STONE}, {1, 1} },

		// wooden pickaxe
        { {{ItemType::PICKAXE, RessourceType::WOOD}, RessourceType::STONE}, {1, 2} },
        { {{ItemType::PICKAXE, RessourceType::WOOD}, RessourceType::IRON},  {1, 1} },

		// wooden axe
        { {{ItemType::AXE, RessourceType::WOOD}, RessourceType::WOOD}, {1, 2} },

		// Iron pickaxe
        { {{ItemType::PICKAXE, RessourceType::STONE}, RessourceType::STONE}, {1, 3} },
        { {{ItemType::PICKAXE, RessourceType::STONE}, RessourceType::IRON},  {1, 2} },

		// Iron axe
        { {{ItemType::AXE, RessourceType::STONE}, RessourceType::WOOD}, {1, 3} },

		// Iron pickaxe
        { {{ItemType::PICKAXE, RessourceType::IRON}, RessourceType::STONE}, {2, 4} },
        { {{ItemType::PICKAXE, RessourceType::IRON}, RessourceType::IRON},  {2, 3} },

		// Iron axe
        { {{ItemType::AXE, RessourceType::IRON}, RessourceType::WOOD}, {2, 4} }
    };

    /* Item names */
    const std::map<ItemCategory, std::string> ITEM_NAMES = {
        { {ItemType::PICKAXE, RessourceType::WOOD},  "Pioche en bois" },
        { {ItemType::PICKAXE, RessourceType::STONE}, "Pioche en pierre" },
        { {ItemType::PICKAXE, RessourceType::IRON},  "Pioche en fer" },
        { {ItemType::AXE, RessourceType::WOOD},      "Hache en bois" },
        { {ItemType::AXE, RessourceType::STONE},     "Hache en pierre" },
        { {ItemType::AXE, RessourceType::IRON},      "Hache en fer" },
        { {ItemType::HAMMER, RessourceType::WOOD},   "Marteau rudimentaire" },
        { {ItemType::HAMMER, RessourceType::STONE},  "Marteau en pierre" },
        { {ItemType::HAMMER, RessourceType::IRON},   "Marteau en fer" }
    };

	/* Structure names */
    const std::map<StructureType, std::string> STRUCTURE_NAMES = {
        { StructureType::FORGE, "Forge" }
    };

    /* Turns to collect Ressources */
	const std::map<RessourceType, int> TURNS_TO_COLLECT = {
        { RessourceType::WOOD, 1 },
        { RessourceType::STONE, 1 },
        { RessourceType::IRON, 1 }
    };

    /* Ressources names */
    const std::map<RessourceType, std::string> RESSOURCE_NAMES = {
        { RessourceType::WOOD,  "Bois" },
        { RessourceType::STONE, "Pierre" },
        { RessourceType::IRON,  "Fer" }
    };
};