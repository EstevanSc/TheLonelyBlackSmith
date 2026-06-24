#include "Gameplay/Entities/Player.h"
#include <stdexcept>

Player::Player(Config& config) : ressourcesManager_(new RessourcesManager(config)), itemsManager_()
{
}

Player::~Player()
{
	delete ressourcesManager_;
	delete itemsManager_;
}

void Player::setScore(int score)
{
	score_ = score;
}

void Player::addScore(int score)
{
	score_ += score;
}

int Player::getScore() const
{
	return score_;
}

ItemsManager* Player::getItemsManager() const
{
	if (!itemsManager_) {
		throw std::runtime_error("Player::getItemsManager() : ItemsManager is not initialized.");
	}
	return itemsManager_;
}

RessourcesManager* Player::getRessourcesManager() const
{
	if (!ressourcesManager_) {
		throw std::runtime_error("Player::getRessourcesManager() : RessourcesManager is not initialized.");
	}
	return ressourcesManager_;
}
