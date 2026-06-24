#include "Gameplay/Components/RessourcesManager.h"

RessourcesManager::RessourcesManager(std::map<RessourceType, int> initialRessources)
{
	ressources_ = initialRessources;
}

RessourcesManager::RessourcesManager(Config& config)
{
	ressources_ = config.INITIAL_RESSOURCES;
}

RessourcesManager::~RessourcesManager()
{
}
