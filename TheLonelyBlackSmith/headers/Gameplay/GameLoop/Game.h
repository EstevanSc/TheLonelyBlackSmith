#pragma once
#include "GameState.h"
#include <Gameplay/Entities/Player.h>
#include <Gameplay/Systems/GatherRessourcesSystem.h>
#include <Gameplay/Systems/CraftSystem.h>

/* Game class managing game loop*/
class Game
{
private:
	/* Game state */
	GameState gameState_;

	/* Player */
	Player* player_;

	/* Current turn */
	int currentTurn_;

	/* Max turns */
	int maxTurns_;

	/* Gather ressources system */
	GatherRessourcesSystem* gatherRessourcesSystem_;

	/* Crafting system */
	CraftSystem* craftSystem_;

	/* Config stored to restart the game */
	Config* config_;

public:
	/* Constructors */
	Game();
	Game(Config& config);
	/* Destructor */
	~Game();

	/* main method */
	void runMainLoop();

	/* method to increase the current turn number*/
	void increaseTurn(int amount = 1);

	/* Getters and Setters */

	// game state
	void setGameState(GameState state);
	GameState getGameState() const;

	// player
	Player* getPlayer() const;

	// current turn
	void setCurrentTurn(int turn);
	int getCurrentTurn() const;

	// max turns
	void setMaxTurns(int maxTurns);
	int getMaxTurns() const;

private:
	/* Method to handle main menu state*/
	void handleMenuState();

	/* Method to handle game state*/
	void handleGameState();

	/* Method to show the gather ressources interface */
	void showGatherRessourcesInterface();

	/* Method to show the craft item interface */
	void showCraftItemInterface();

	/* Method to show the ressources display interface */
	void showRessourcesDisplayInterface();

	/* Method to show the game Over menu*/
	void showGameOverMenu();

	/* Method to reset the game */
	void resetGame();
};