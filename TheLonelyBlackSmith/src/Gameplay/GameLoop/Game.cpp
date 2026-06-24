#include <Gameplay/Systems/BotSystem.h>
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
	botSystem_ = new BotSystem();
	config_ = &config;
}

Game::Game(Config& config) : player_(new Player(config)), currentTurn_(0), maxTurns_(config.MAX_TURNS), 
gameState_(GameState::MENU), gatherRessourcesSystem_(new GatherRessourcesSystem(config)), craftSystem_(new CraftSystem(config)),
config_(&config), botSystem_(new BotSystem())
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
    if (botSystem_) {
        delete botSystem_;
        botSystem_ = nullptr;
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
            case GameState::BOTPLAYING:
				botSystem_->playTurn(*this, *player_, *craftSystem_, *gatherRessourcesSystem_);
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
        if (currentTurn_ >= maxTurns_ || !hasPossibleActions()) {
			currentTurn_ = maxTurns_;
			showRessourcesDisplayInterface();
            gameState_ = GameState::GAMEOVER;
		}
    }
    else {
		showRessourcesDisplayInterface();
        gameState_ = GameState::GAMEOVER;
	}
}

bool Game::canIncreaseTurn(int amount) const
{
    if (currentTurn_ + amount <= maxTurns_) {
        return true;
	}
	return false;
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
        std::cout << "1. Commencer la partie" << std::endl;
        std::cout << "2. Mode Bot" << std::endl;
        std::cout << "3. Quitter le jeu" << std::endl;
        std::cout << "Choix: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrée invalide, entrez un nombre entre 1 et 2." << std::endl;
            continue;
        }

        switch (choice) {
        case 1:
            gameState_ = GameState::GAME;
            break;
        case 2:
            gameState_ = GameState::BOTPLAYING;
            break;
        case 3:
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
        std::cout << "\n--- Tour " << currentTurn_+1 << "/" << maxTurns_ << " ---" << std::endl;
        std::cout << "Score: " << player_->getScore() << std::endl;
        std::cout << "1. Collecter des ressources" << std::endl;
        std::cout << "2. Fabriquer un objet" << std::endl;
        std::cout << "3. Afficher les ressources actuelles" << std::endl;
        std::cout << "4. Retour au menu principal" << std::endl;
        std::cout << "Choix: ";

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
            std::cout << "Choix invalide." << std::endl;
            return;
        }
	}
}

void Game::showRessourcesDisplayInterface()
{
    if (!player_) {
        throw std::runtime_error("Game::showRessourcesDisplayInterface() : Player is not initialized.");
        return;
	}
    if (!player_->getRessourcesManager()) {
        throw std::runtime_error("Game::showRessourcesDisplayInterface() : RessourcesManager is not initialized.");
		return;
    }
    if (!player_->getItemsManager()) {
        throw std::runtime_error("Game::showRessourcesDisplayInterface() : ItemsManager is not initialized.");
    }

    std::cout << "\n--- Inventaire ---" << std::endl;
	RessourcesManager* resManager = player_->getRessourcesManager();
    ItemsManager* itemsManager = player_->getItemsManager();

	resManager->showRessources();
	itemsManager->showOwnedTools();
	craftSystem_->showConstructedStructures();

}

void Game::showGameOverMenu()
{
    std::cout << "\n--- Fin de partie ---" << std::endl;
    std::cout << "Votre score final : " << player_->getScore() << std::endl;
    std::cout << "1. Retour au menu principal" << std::endl;
    std::cout << "2. Quitter le jeu" << std::endl;
    std::cout << "Choix: ";
    int choice;
    std::cin >> choice;
    switch (choice) {
    case 1:
        resetGame();
        break;
    case 2:
        gameState_ = GameState::QUIT;
        break;
    default:
        std::cout << "Choix invalide." << std::endl;
        break;
	}
}

void Game::resetGame()
{
    currentTurn_ = 0;
    if (player_) {
        delete player_;
    }
    if (gatherRessourcesSystem_) {
        delete gatherRessourcesSystem_;
	}
    if (craftSystem_) {
        delete craftSystem_;
	}
    if (botSystem_) {
        delete botSystem_;
    }
    if (config_) {
        player_ = new Player(*config_);
		gatherRessourcesSystem_ = new GatherRessourcesSystem(*config_);
		craftSystem_ = new CraftSystem(*config_);
        botSystem_ = new BotSystem(*config_);
		
    }
    else {
        player_ = new Player();
		gatherRessourcesSystem_ = new GatherRessourcesSystem();
		craftSystem_ = new CraftSystem();
		botSystem_ = new BotSystem();
	}
	gameState_ = GameState::MENU;
}

bool Game::hasPossibleActions() const
{
    if (!player_) {
        throw std::runtime_error("Game::hasPossibleActions() : Player is not initialized.");
        return false;
    }
    if (!gatherRessourcesSystem_) {
        throw std::runtime_error("Game::hasPossibleActions() : GatherRessourcesSystem is not initialized.");
        return false;
	}
    if (!craftSystem_) {
        throw std::runtime_error("Game::hasPossibleActions() : CraftSystem is not initialized.");
		return false;
	}

	int minTurnsToGather = gatherRessourcesSystem_->getMinimumTurnsToGather();
	int minTurnsToCraft = craftSystem_->getMinimumTurnToCraft(*player_);
	int remainingTurns = maxTurns_ - currentTurn_;
    if (minTurnsToGather <= remainingTurns || minTurnsToCraft <= remainingTurns) {
        return true;
    }
	std::cout << "Aucune action possible, vous n'avez pas assez de tours restants pour collecter ou fabriquer." << std::endl;
    return false;
}

void Game::showGatherRessourcesInterface()
{
    if (!gatherRessourcesSystem_) {
        throw std::runtime_error("Game::showGatherRessourcesInterface() : GatherRessourcesSystem is not initialized.");
        return;
	}
    std::cout << "\nQuelle ressource collecter ?" << std::endl;
	gatherRessourcesSystem_->showGatherOptions(*player_);
	int indexOfCancelOption = gatherRessourcesSystem_->getNumberOfGatherOptions() + 1;
	std::cout << indexOfCancelOption << ". Retour au menu du tour" << std::endl;
    std::cout << "Choix: ";

    int resChoice;
    std::cin >> resChoice;
    if (resChoice < 1 || resChoice > indexOfCancelOption) {
        std::cout << "Choix invalide. Retour au menu du tour" << std::endl;
        return;
	}
    if (resChoice == indexOfCancelOption) {
        return;
    }
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

    std::cout << "\n--- Recettes disponibles ---" << std::endl;
	craftSystem_->showCraftList(*player_);
	int numberOfCrafts = craftSystem_->getNumberOfCrafts();
	std::cout << numberOfCrafts + 1 << ". Retour au menu du tour" << std::endl;
	std::cout << "Choix: ";

    int craftChoice;
    std::cin >> craftChoice;
    if (craftChoice >= 1 && craftChoice <= numberOfCrafts) {
		craftSystem_->craftByChoice(*this, *player_, std::to_string(craftChoice));
    }
    else if (craftChoice == numberOfCrafts + 1) {
        return;
    }
    else {
        std::cout << "Choix invalide. Retour au menu du tour." << std::endl;
        return;
	}
}