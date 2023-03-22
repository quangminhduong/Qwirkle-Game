#include "Board.h"

// Constructor
// Creates an empty board with a board edge all around it which is used to check edge cases
Board::Board(int boardDimension) {
    this->boardDimension = boardDimension;
    std::vector<std::string> firstRow;
    for (int i = 0; i < boardDimension+2; i++) {
        firstRow.push_back(BOARD_EDGE);
    }
    board.push_back(firstRow);

    for (int i = 0; i < boardDimension; i++) {
        std::vector<std::string> row;
        row.push_back(BOARD_EDGE);
        for (int j = 0; j < boardDimension; j++) {
            row.push_back("  ");
        }
        row.push_back(BOARD_EDGE);
        board.push_back(row);
    }
    std::vector<std::string> finalRow;
    for (int i = 0; i < boardDimension+2; i++) {
        finalRow.push_back(BOARD_EDGE);
    }
    board.push_back(finalRow);
    isEmpty = true;
}

// Destructor
Board::~Board() {
    for (int i = 0; i < boardDimension+2; i++) {
        board[i].clear();
    }
    board.clear();
}

// Check if the move is valid
bool Board::isValidMove(std::string tile, std::string position) {
    int rowIndex = getRowIndex(position[0])+1;
    int colIndex = std::stoi(position.substr(1))+1;

    bool isValid = false;

    if (rowIndex >= 0 && rowIndex <= boardDimension+1 && colIndex >= 0 && 
        colIndex <= boardDimension+1 && board[rowIndex][colIndex] == "  ") {

        if (!isEmpty) { // Check surrounding tiles
            if (hasVerticalAndHorizontalNeighbours(position)){
                if (checkRows(tile, position) && checkColumns(tile, position)) {
                    isValid = true;
                }
            } else if (checkRows(tile, position)) {
                isValid = true;
            } else if (checkColumns(tile, position)) {
                isValid = true;
            }

        } else { // First tile placement of game
            isValid = true;
            isEmpty = false;
            points++;
        }
    }

    return isValid;
}

// Place the tile on the square
void Board::placeTileOnSquare(std::string tile, std::string position) {
    int rowIndex = getRowIndex(position[0])+1;
    int colIndex = std::stoi(position.substr(1))+1;

    board[rowIndex][colIndex] = tile;
    isEmpty = false;
}

// Get the board state as string representation
std::string Board::getBoardStateAsString()
{
    std::string boardState = "";
    char letter = 'A';
    for (int i = 1; i <= boardDimension; i++) {
        for (int j = 1; j <= boardDimension; j++) {
            if (board[i][j] != "  ") {
                boardState += board[i][j] + "@" + letter + std::to_string(j-1) + ",";
            }
        }
        letter++;
    }
    return boardState.substr(0, boardState.size() - 1);
}

// Get the dimension of the board
int Board::getBoardDimension() {
    return boardDimension;
}

// Display the board
void Board::printBoard() {
    std::string topLabel = "   ";
    std::string topLine = "  _";
    for (int i = 0; i < boardDimension; i++) {
        topLabel += (std::to_string(i) + "  ").substr(0, 3);
        topLine += "___";
    }

    std::cout << topLabel << std::endl;
    std::cout << topLine << std::endl;

    char letter = 'A';
    for (int i = 1; i <= boardDimension; i++) {
        std::cout << letter << " |";
        for (int j = 1; j <= boardDimension; j++) {
            std::cout << board[i][j] << "|";
        }
        std::cout << std::endl;
        letter++;
    }
    std::cout << std::endl;
}

// Get the row index of the row label
int Board::getRowIndex(char rowLabel) {
    char letter = 'A';

    int rowIndex = -1;
    for (int i = 0; i < boardDimension; i++) {
        if (rowLabel == letter) {
            rowIndex = i;
        }
        letter++;
    }
    return rowIndex;
}

// Check rows
bool Board::checkRows(std::string tile, std::string position) {
    int rowIndex = getRowIndex(position[0]) + 1;
    int colIndex = std::stoi(position.substr(1)) + 1;

    std::vector<std::string> rowOfSameColourTiles; // Vector to store same colour tiles

    if (board[rowIndex][colIndex-1] != BOARD_EDGE && board[rowIndex][colIndex-1][0] == tile[0] && 
        board[rowIndex][colIndex+1] != BOARD_EDGE && board[rowIndex][colIndex+1][0] == tile[0]) // Tiles on left and right
    {
        while (board[rowIndex][colIndex-1][0] == tile[0]) { // Add tiles to left with same colour in vector
            rowOfSameColourTiles.push_back(board[rowIndex][colIndex-1]);
            colIndex--; 
        }

        int colIndex = std::stoi(position.substr(1)) + 1; // Reset col index to move right

        while (board[rowIndex][colIndex+1][0] == tile[0]) { // Add tiles to right with same colour in vector
            rowOfSameColourTiles.push_back(board[rowIndex][colIndex+1]);
            colIndex++; 
        }
    }
    else if (board[rowIndex][colIndex+1] != BOARD_EDGE && board[rowIndex][colIndex+1][0] == tile[0])// Only tiles on right
    {
        while (board[rowIndex][colIndex+1][0] == tile[0]) { // Tiles on right are same colour
            rowOfSameColourTiles.push_back(board[rowIndex][colIndex+1]);
            colIndex++; 
        }
    }
    else if (board[rowIndex][colIndex-1] != BOARD_EDGE && board[rowIndex][colIndex-1][0] == tile[0]) // Only tiles on left
    {
        while (board[rowIndex][colIndex-1][0] == tile[0]) { // Tiles on left are same colour
            rowOfSameColourTiles.push_back(board[rowIndex][colIndex-1]);
            colIndex--; 
        }
    }

    bool validMove = false; // Check the tile is a different shape from neighbours
    if (rowOfSameColourTiles.size() > 0 && rowOfSameColourTiles.size() < 6) {
        validMove = true;
        for (long unsigned int i = 0; i < rowOfSameColourTiles.size(); i++) {
            if (rowOfSameColourTiles[i][1] == tile[1]) {
                validMove = false;
            }
        }
        
        points += rowOfSameColourTiles.size() + 1; // Number of tiles next to placed tile plus itself
        if (rowOfSameColourTiles.size() == 5) { // Five tiles in a row plus placed tile is quirkle
            quirkle = true;
            points += 6;
        }
    }

    return validMove;
}

// Check columns
bool Board::checkColumns(std::string tile, std::string position) {
    int rowIndex = getRowIndex(position[0]) + 1;
    int colIndex = std::stoi(position.substr(1)) + 1;

    std::vector<std::string> colsOfSameShapedTiles; // Vector to store same shaped tiles

    if (board[rowIndex+1][colIndex] != BOARD_EDGE && board[rowIndex+1][colIndex][1] == tile[1] && 
        board[rowIndex-1][colIndex] != BOARD_EDGE && board[rowIndex-1][colIndex][1] == tile[1]) // Tiles above and below
    {
        while (board[rowIndex+1][colIndex][1] == tile[1]) { // Add tiles below in vector
            colsOfSameShapedTiles.push_back(board[rowIndex+1][colIndex]);
            rowIndex++; 
        }

        int rowIndex = getRowIndex(position[0]) + 1; // Reset row index to move up

        while (board[rowIndex-1][colIndex][1] == tile[1]) { // Add tiles above in vector
            colsOfSameShapedTiles.push_back(board[rowIndex-1][colIndex]);
            rowIndex--; 
        }
    }
    else if (board[rowIndex+1][colIndex] != BOARD_EDGE && board[rowIndex+1][colIndex][1] == tile[1])// Only tiles below
    {
        while (board[rowIndex+1][colIndex][1] == tile[1]) { // Tiles below with same shape
            colsOfSameShapedTiles.push_back(board[rowIndex+1][colIndex]);
            rowIndex++; 
        }
    }
    else if (board[rowIndex-1][colIndex] != BOARD_EDGE && board[rowIndex-1][colIndex][1] == tile[1]) // Only tiles above
    {
        while (board[rowIndex-1][colIndex][1] == tile[1]) { // Tiles above with same shape
            colsOfSameShapedTiles.push_back(board[rowIndex-1][colIndex]);
            rowIndex--; 
        }
    }

    bool validMove = false;
    if (colsOfSameShapedTiles.size() > 0 && colsOfSameShapedTiles.size() < 6) {
        validMove = true;
        for (long unsigned int i = 0; i < colsOfSameShapedTiles.size(); i++) {
            if (colsOfSameShapedTiles[i][0] == tile[0]) {
                validMove = false;
            }
        }
        
        points += colsOfSameShapedTiles.size() + 1;
        if (colsOfSameShapedTiles.size() == 5) {
            quirkle = true;
            points += 6;
        }
    }

    return validMove;
}

// Check if the position has vertical and horizontal neighbours
bool Board::hasVerticalAndHorizontalNeighbours(std::string position) {
    int rowIndex = getRowIndex(position[0]) + 1;
    int colIndex = std::stoi(position.substr(1)) + 1;

    bool hasVertAndHoriNeighbour = false;
    // Check left and down
    if (board[rowIndex][colIndex-1] != BOARD_EDGE && board[rowIndex][colIndex-1] != "  " && 
        board[rowIndex+1][colIndex] != BOARD_EDGE && board[rowIndex+1][colIndex] != "  ") {
        hasVertAndHoriNeighbour = true;
    }
    // Check left and up
    if (board[rowIndex][colIndex-1] != BOARD_EDGE && board[rowIndex][colIndex-1] != "  " && 
        board[rowIndex-1][colIndex] != BOARD_EDGE && board[rowIndex-1][colIndex] != "  ") {
        hasVertAndHoriNeighbour = true;
    }
    // Check right and down
    if (board[rowIndex][colIndex+1] != BOARD_EDGE && board[rowIndex][colIndex+1] != "  " && 
        board[rowIndex+1][colIndex] != BOARD_EDGE && board[rowIndex+1][colIndex] != "  ") {
        hasVertAndHoriNeighbour = true;
    }
    // Check right and up
    if (board[rowIndex][colIndex+1] != BOARD_EDGE && board[rowIndex][colIndex+1] != "  " && 
        board[rowIndex-1][colIndex] != BOARD_EDGE && board[rowIndex-1][colIndex] != "  ") {
        hasVertAndHoriNeighbour = true;
    }

    return hasVertAndHoriNeighbour;
}

// Scan entire board for possible moves
bool Board::checkAllValidMoves(std::string tile) {
    bool isValid = false;
    char letter = 'A';
    for (int i = 0; i < boardDimension; i++) {
        for (int j = 0; j < boardDimension; j++) {
            std::string position = std::string(1,letter) + std::to_string(j);
            if (hasVerticalAndHorizontalNeighbours(position)){
                if (board[i+1][j+1] == "  " && checkRows(tile, position) && checkColumns(tile, position)) {
                    isValid = true;
                }
            } 
            else if (board[i+1][j+1] == "  " && checkRows(tile, position)) {
                isValid = true;
            } 
            else if (board[i+1][j+1] == "  " && checkColumns(tile, position)) {
                isValid = true;
            }
        }
        letter++;
    }
    return isValid;
}

int Board::getPoints() {
    return points;
}

void Board::resetPoints() {
    points = 0;
}

bool Board::isQuirkle() {
    return quirkle;
}

void Board::resetQuirkle() {
    quirkle = false;
}