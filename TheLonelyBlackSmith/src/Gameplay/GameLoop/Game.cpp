#include "Gameplay/GameLoop/Game.h"
#include <stdexcept>
#include <iostream>

Game::Game()
{
	Config config;
	player_ = new Player(config);
	currentTurn_ = 0;
	maxTurns_ = config.MAX_TURNS;
	gameState_ = GameState::MENU;
	gatherRessourcesSystem_ = new GatherRessourcesSystem(config);
	craftSystem_ = new CraftSystem(config);
}

Game::Game(Config& config) : player_(new Player(config)), currentTurn_(0), maxTurns_(config.MAX_TURNS), 
gameState_(GameState::MENU), gatherRessourcesSystem_(new GatherRessourcesSystem(config)), craftSystem_(new CraftSystem(config))
{

}

Game::~Game()
{
	if (player_) {
		delete player_;
		player_ = nullptr;
	}
    if (gatherRessourcesSystem_) {
		delete gatherRessourcesSystem_;
		gatherRessourcesSystem_ = nullptr;
    }
    if (craftSystem_) {
        delete craftSystem_;
        craftSystem_ = nullptr;
    }
}

void Game::runMainLoop()
{
    while (true) { // main loop, handle the whole game loop
        switch (gameState_) {
            case GameState::MENU:
                handleMenuState();
                break;
            case GameState::GAME:
			    handleGameState();
                break;
            case GameState::GAMEOVER:
			    showGameOverMenu();
                break;
            case GameState::QUIT:
                std::cout << "Exiting the game." << std::endl;
                return;
            default:
                throw std::runtime_error("Invalid game state.");
            }
	}
}

void Game::increaseTurn(int amount)
{
    if (currentTurn_ < maxTurns_) {
        currentTurn_+= amount;
    }
    else {
        gameState_ = GameState::GAMEOVER;
	}
}

void Game::setGameState(GameState state)
{
	gameState_ = state;
}

GameState Game::getGameState() const
{
	return GameState();
}

Player* Game::getPlayer() const
{
	if (!player_) {
		throw std::runtime_error("Game::getPlayer() : Player is not initialized.");
	}
	return player_;
}

void Game::setCurrentTurn(int turn)
{
	currentTurn_ = turn;
}

int Game::getCurrentTurn() const
{
	return currentTurn_;
}

void Game::setMaxTurns(int maxTurns)
{
	maxTurns_ = maxTurns;
}

int Game::getMaxTurns() const
{
	return maxTurns_;
}

void Game::handleMenuState()
{
    int choice = 0;
    while (gameState_ == GameState::MENU) {
        std::cout << "\n--- THE LONELY BLACKSMITH ---" << std::endl;
        std::cout << "1. Start Game" << std::endl;
        std::cout << "2. Quit Game" << std::endl;
        std::cout << "Choice: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input, enter a number between 1 and 2." << std::endl;
            continue;
        }

        switch (choice) {
        case 1:
            gameState_ = GameState::GAME;
            break;
        case 2:
            gameState_ = GameState::QUIT;
            break;
        default:
			throw std::runtime_error("Invalid menu choice. Please enter 1 or 2.");
        }
    }
}
void Game::handleGameState()
{
    int choice = 0;
    if (!player_) {
        throw std::runtime_error("Game::handleGameState() : Player is not initialized.");
		return;
	}
    if (!gatherRessourcesSystem_) {
        throw std::runtime_error("Game::handleGameState() : GatherRessourcesSystem is not initialized.");
        return;
    }
    if (!craftSystem_) {
        throw std::runtime_error("Game::handleGameState() : CraftSystem is not initialized.");
        return;
	}
    while (gameState_ == GameState::GAME) {
        std::cout << "\n--- Turn " << currentTurn_ << "/" << maxTurns_ << " ---" << std::endl;
		std::cout << "Score: " << player_->getScore() << std::endl;
        std::cout << "1. Gather Resources" << std::endl;
        std::cout << "2. Craft Item" << std::endl;
        std::cout << "3. Show Current ressources" << std::endl;
        std::cout << "4. Back to Main Menu" << std::endl;
        std::cout << "Choice: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }

        switch (choice) {
        case 1:
            showGatherRessourcesInterface();
            break;
        case 2:
            showCraftItemInterface();
            break;
        case 3:
            showRessourcesDisplayInterface();
            break;
        case 4:
            gameState_ = GameState::MENU;
            break;
        default:
            std::cout << "Invalid choice." << std::endl;
            return;
        }
	}
}

void Game::showRessourcesDisplayInterface()
{
    std::cout << "\n--- Current Inventory ---" << std::endl;
	RessourcesManager* resManager = player_->getRessourcesManager();
    const auto& inventory = resManager->getRessources();
    
    if (resManager) {
		const auto& inventory = resManager->getRessources();

        std::cout << "Wood: " << inventory.at(RessourceType::WOOD) << " | "
            << "Stone: " << inventory.at(RessourceType::STONE) << " | "
            << "Iron: " << inventory.at(RessourceType::IRON) << std::endl;
    }
    else {
		throw std::runtime_error("Game::showRessourcesDisplayInterface() : RessourcesManager is not initialized.");
    }
}

void Game::showGameOverMenu()
{
    std::cout << "\n--- Game Over ---" << std::endl;
    std::cout << "Your final score: " << player_->getScore() << std::endl;
    std::cout << "1. Return to Main Menu" << std::endl;
    std::cout << "2. Quit Game" << std::endl;
    std::cout << "Choice: ";
    int choice;
    std::cin >> choice;
    switch (choice) {
    case 1:
        gameState_ = GameState::MENU;
        break;
    case 2:
        gameState_ = GameState::QUIT;
        break;
    default:
        std::cout << "Invalid choice." << std::endl;
        break;
	}
}

void Game::showGatherRessourcesInterface()
{
    std::cout << "\nWhich resource to gather?" << std::endl;
    std::cout << "1. Wood | 2. Stone | 3. Iron" << std::endl;
    std::cout << "Choice: ";

    int resChoice;
    std::cin >> resChoice;

    RessourceType target = static_cast<RessourceType>(resChoice);
	gatherRessourcesSystem_->gatherRessources(*this, *player_, target);
}

void Game::showCraftItemInterface()
{
    if (!craftSystem_) {
        throw std::runtime_error("Game::showCraftItemInterface() : CraftSystem is not initialized.");
        return;
	}
    if (!player_) {
        throw std::runtime_error("Game::showCraftItemInterface() : Player is not initialized.");
        return;
    }

    std::cout << "\n--- Available Recipes ---" << std::endl;
	craftSystem_->showCraftList(*player_);
	int numberOfCrafts = craftSystem_->getNumberOfCrafts();
	std::cout << numberOfCrafts + 1 << ". Back to Game Menu" << std::endl;
	std::cout << "Choice: ";

    int craftChoice;
    std::cin >> craftChoice;
    if (craftChoice >= 1 && craftChoice <= numberOfCrafts) {
		craftSystem_->craftByChoice(*this, *player_, std::to_string(craftChoice));
    }
    else if (craftChoice == numberOfCrafts + 1) {
        return;
    }
    else {
        std::cout << "Invalid choice." << std::endl;
	}
}