#include <Gameplay/Systems/BotSystem.h>
#include <Gameplay/GameLoop/Game.h>
#include <iostream>
#include <vector>
#include <algorithm>

BotSystem::BotSystem()
{
	Config config;
    buildItemCraftPriority(config.ITEM_RECIPES);
    buildStructureCraftPriority(config.STRUCTURE_RECIPES);
}

BotSystem::BotSystem(Config& config)
{
    buildItemCraftPriority(config.ITEM_RECIPES);
    buildStructureCraftPriority(config.STRUCTURE_RECIPES);
}

void BotSystem::playTurn(Game& game, Player& player, CraftSystem& craftSystem, GatherRessourcesSystem& gatherSystem)
{
	// We handle that as a behavior tree, first we try to craft, if we can't craft we gather the most needed ressource
	bool crafted = tryToCraft(game, player, craftSystem);
    if (!crafted) {
        RessourceType target = getMostNeededRessource(player, craftSystem, gatherSystem);
        gatherSystem.gatherRessources(game, player, target);
	}
}

bool BotSystem::tryToCraft(Game& game, Player& player, CraftSystem& craftSystem)
{
    for (const auto& structureType : structureCraftPriority_) {
        if (craftSystem.canCraftStructure(player, structureType)) {
            return craftSystem.craftStructure(game, player, structureType);
        }
    }
    for (const auto& itemCategory : itemCraftPriority_) {
        if (craftSystem.canCraftItem(player, itemCategory)) {
            return craftSystem.craftItem(game, player, itemCategory);
        }
    }
    return false;
}

RessourceType BotSystem::getMostNeededRessource(Player& player, CraftSystem& craftSystem, GatherRessourcesSystem& gatherSystem)
{
    RessourcesManager* rm = player.getRessourcesManager();
    RessourceType mostNeeded = RessourceType::NONE;
    int valueOwned = -1;
    for (int i = 1; i < static_cast<int>(RessourceType::COUNT); i++) {
        RessourceType ressourceType = static_cast<RessourceType>(i);
        if (gatherSystem.canGatherRessource(player, ressourceType)) {
            int owned = rm->getRessource(ressourceType);
            if (valueOwned == -1 || owned < valueOwned) {
                valueOwned = owned;
                mostNeeded = ressourceType;
            }
        }
    }
    if (mostNeeded == RessourceType::NONE) {
        return RessourceType::WOOD;
    }
    return mostNeeded;
}

void BotSystem::buildItemCraftPriority(const std::map<ItemCategory, Recipe> itemRecipes)
{
    itemCraftPriority_.clear();
    std::vector<std::pair<ItemCategory, Recipe>> sorted(itemRecipes.begin(), itemRecipes.end());
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return a.second.score_ > b.second.score_;
        });
    for (const auto& [itemCategory, recipe] : sorted) {
        itemCraftPriority_.push_back(itemCategory);
    }
}

void BotSystem::buildStructureCraftPriority(const std::map<StructureType, Recipe> structureRecipes)
{
    structureCraftPriority_.clear();
    std::vector<std::pair<StructureType, Recipe>> sorted(structureRecipes.begin(), structureRecipes.end());
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return a.second.score_ > b.second.score_;
        });
    for (const auto& [structureType, recipe] : sorted) {
        structureCraftPriority_.push_back(structureType);
    }
}