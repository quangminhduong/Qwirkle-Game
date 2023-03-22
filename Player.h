#ifndef PLAYER_H
#define PLAYER_H 

#include "LinkedList.h"
#include <iostream>

class Player {
public:
    Player(std::string name);
    ~Player();

    std::string getName();
    void addToBackOfHand(Tile* tile);
    void addScore(int points);
    int getScore();
    Tile* getTile(std::string tileCode);
    Tile* getTile(int index);
    std::string getHandAsString();

    bool tileInHand(std::string tileCode);
    void deleteTile(std::string tileCode);
    int handSize();
    bool handIsEmpty();

private:
    std::string name;
    LinkedList* hand;
    int score;
};
#endif // PLAYER_H
