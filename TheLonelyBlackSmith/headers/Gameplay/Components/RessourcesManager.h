#pragma once
#include <map>
#include <Config/Config.h>


/* RessourcesManager component to handle ressources inventory behaviour */
class RessourcesManager
{
public:
	std::map<RessourceType, int> ressources_;

public:
	/* Constructors */
	RessourcesManager(std::map<RessourceType, int> initialRessources = {});
	RessourcesManager(Config& config);

	/* Destructor */
	~RessourcesManager();
};

