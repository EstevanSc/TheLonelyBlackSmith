#pragma once
#include <map>
#include <Config/Config.h>


/* RessourcesManager component to handle ressources inventory behaviour */
class RessourcesManager
{
private:
	/* ressources inventory */
	std::map<RessourceType, int> ressources_;

	/* ressources names */
	std::map<RessourceType, std::string> ressourcesNames_;

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

	/* Method to show current ressources */
	void showRessources() const;

	/* Getter for ressources */
	const std::map<RessourceType, int>& getRessources() const;
};

