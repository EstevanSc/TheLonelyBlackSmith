// main file for the project 

#include <Config/Config.h>
#include <Gameplay/GameLoop/Game.h>
#include <Windows.h>

int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	// Initialize the game configuration
	Config config;

	// Create a Game instance
	Game game(config);

	// Run the main game loop
	game.runMainLoop();

	return 0;
}