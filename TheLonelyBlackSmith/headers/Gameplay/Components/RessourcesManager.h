#pragma once
#include <map>
#include <Config/Config.h>


/* RessourcesManager component to handle ressources inventory behaviour */
class RessourcesManager
{
private:
	std::map<RessourceType, int> ressources_;

public:
	/* Constructors */
	RessourcesManager(std::map<RessourceType, int> initialRessources = {});
	RessourcesManager(Config& config);

	/* Destructor */
	~RessourcesManager();
	
	/* Method to add ressources */
	void addRessource(RessourceType type, int amount);

	/* Method to check if ressources amount is enough */
	bool hasEnoughRessources(RessourceType type, int amount) const;

	/* Method to remove ressources */
	void removeRessource(RessourceType type, int amount);

	/* Getter for ressources */
	const std::map<RessourceType, int>& getRessources() const;
};

