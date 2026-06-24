#pragma once
#include <Config/Config.h>
#include <Gameplay/Components/ItemsManager.h>
#include <Gameplay/Components/RessourcesManager.h>

class Player
{
private:
	/* Items manager component to handle items inventory*/
	ItemsManager* itemsManager_;

	/* Ressources manager component to handle ressources inventory*/
	RessourcesManager* ressourcesManager_;

	/* Score of the player */
	int score_;

public:
	/* Constructors */
	Player(Config& config);

	/* Destructor */
	~Player();

	// Getters and Setters
	// score
	void setScore(int score);
	void addScore(int score);
	int getScore() const;
	
	// items manager
	ItemsManager* getItemsManager() const;

	// ressources manager
	RessourcesManager* getRessourcesManager() const;

};

