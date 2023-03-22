#include "Player.h"

// Constructor
Player::Player(std::string name) {
    this->name = name;
    score = 0;
    hand = new LinkedList();
}

// Destructor
Player::~Player() {
    delete hand;
}

// Get the name of the player
std::string Player::getName() {
    return name;
}

// Add the tile to the back of hand
void Player::addToBackOfHand(Tile* tile) {
    hand->addBack(tile);
}

// Add point to score
void Player::addScore(int points) {
    score += points;
}

// Get the score
int Player::getScore() {
    return score;
}

// Get the tile by tile code
Tile* Player::getTile(std::string tileCode) {
    int index = 0;
    for (int i = 0; i < hand->size(); i++) {
        if (tileCode == hand->get(i)->getColour() + std::to_string(hand->get(i)->getShape())) {
            index = i;
        }
    }
    return hand->get(index);
}

// Get the tile by index
Tile* Player::getTile(int index) {
    return hand->get(index);
}

// Get all tile in the hand as string representation
std::string Player::getHandAsString() {
    std::string handString;
    for (int i = 0; i < hand->size(); i++) {
        handString += hand->get(i)->getColour() + std::to_string(hand->get(i)->getShape()) + ",";
    }
    return handString.substr(0,handString.size()-1);
}

// Check if the tile is in hand
bool Player::tileInHand(std::string tileCode) {
    bool inHand = false;
    for (int i = 0; i < hand->size(); i++) {
        if (tileCode == hand->get(i)->getColour() + std::to_string(hand->get(i)->getShape())) {
            inHand = true;
        }
    }
    return inHand;
}

// Delete the tile
void Player::deleteTile(std::string tileCode) {
    bool deleted = false;
    for (int i = 0; i < hand->size(); i++) {
        if (tileCode == hand->get(i)->getColour() + std::to_string(hand->get(i)->getShape()) && !deleted) {
            hand->deleteAt(i);
            deleted = true; //to prevent a double getting deleted
        }
    }
}

int Player::handSize() {
    return hand->size();
}

bool Player::handIsEmpty() {
    return hand->size() == 0;
}