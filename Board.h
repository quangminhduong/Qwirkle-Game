#ifndef BOARD_H
#define BOARD_H 

#include <vector>
#include <string> 
#include <iostream>

#define BOARD_DIMENSION     26
#define BOARD_EDGE          "Q2C"

class Board {
public:
    Board(int boardDimension);
    ~Board();
    // Check tile is placed at a valid location
    bool isValidMove(std::string tile, std::string position);
    void placeTileOnSquare(std::string tile, std::string position);
    // Return board state as a string for save file
    std::string getBoardStateAsString();
    int getBoardDimension();
    void printBoard();
    // Check if there are same coloured tiles to left and right
    bool checkRows(std::string tile, std::string position);
    // Check if there are same shaped tiles above and below
    bool checkColumns(std::string tile, std::string position);
    // Check if there are both vertical and horizontal neighbours
    bool hasVerticalAndHorizontalNeighbours(std::string position);
    // Scane entire board for possible moves
    bool checkAllValidMoves(std::string tile);
    int getPoints();
    void resetPoints();
    bool isQuirkle();
    void resetQuirkle();

private:
    int boardDimension;
    int getRowIndex(char rowLabel);
    bool isEmpty;
    
    int points = 0;
    
    bool quirkle = false;
    
    std::vector<std::vector<std::string> > board;
};

#endif // BOARD_H
