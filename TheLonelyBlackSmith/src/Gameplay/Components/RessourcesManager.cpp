#include "Gameplay/Components/RessourcesManager.h"
#include <stdexcept>

RessourcesManager::RessourcesManager(std::map<RessourceType, int> initialRessources)
{
	ressources_ = initialRessources;
	Config config;
	ressourcesNames_ = config.RESSOURCE_NAMES;
}

RessourcesManager::RessourcesManager(Config& config) : ressourcesNames_(config.RESSOURCE_NAMES)
{
	ressources_ = config.INITIAL_RESSOURCES;
}

RessourcesManager::~RessourcesManager()
{
}

void RessourcesManager::addRessource(RessourceType type, int amount)
{
	if (type == RessourceType::NONE) {
		throw std::invalid_argument("Cannot add NONE ressource type.");
	}
	else if (amount < 0) {
		throw std::invalid_argument("Cannot add negative amount of ressources.");
	}

	if (ressources_.find(type) == ressources_.end()) {
		ressources_[type] = amount;
	}
	else {
		ressources_[type] += amount;
	}
}

bool RessourcesManager::hasEnoughRessources(RessourceType type, int amount) const
{
	if (ressources_.find(type) == ressources_.end()) {
		return false; // 0
	}
	return ressources_.at(type) >= amount;
}

void RessourcesManager::removeRessource(RessourceType type, int amount)
{
	if (type == RessourceType::NONE) {
		throw std::invalid_argument("Cannot remove NONE ressource type.");
	}
	else if (amount < 0) {
		throw std::invalid_argument("Cannot remove negative amount of ressources.");
	}
	if (!hasEnoughRessources(type, amount)) {
		throw std::runtime_error("Not enough ressources to remove.");
	}
	if (ressources_.find(type) == ressources_.end()) {
		throw std::runtime_error("Ressource type not found.");
	}
	ressources_[type] -= amount;
}

void RessourcesManager::showRessources() const
{
	std::cout << "\nCurrent Ressources:" << std::endl;
	for (const auto& pair : ressources_) {
		if (pair.first == RessourceType::NONE) {
			continue;
		}
		if (ressourcesNames_.find(pair.first) == ressourcesNames_.end()) {
			throw std::runtime_error("Ressource name not found for type.");
			continue;
		}
		std::string ressourceName = ressourcesNames_.at(pair.first);
		std::cout << ressourceName << ": " << pair.second << std::endl;
	}
}

const std::map<RessourceType, int>& RessourcesManager::getRessources() const
{
	return ressources_;
}
