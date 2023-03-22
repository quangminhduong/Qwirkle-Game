#include "GameMenu.h"



GameMenu::GameMenu(){
}
GameMenu::~GameMenu(){
}
void GameMenu::menu(bool &exit) {
   std::cout << "Menu" << std::endl;
   std::cout << "----" << std::endl;
   std::cout << "1. New Game" << std::endl;
   std::cout << "2. Load Game" << std::endl;
   std::cout << "3. Credits" << std::endl;
   std::cout << "4. Quit" << std::endl << std::endl << "> ";
   
   std::string menuInput;
   std::getline(std::cin, menuInput);
   checkEmptyInput(menuInput);
   if (checkEOF(exit)) {} //Exit
   else {
      while ((!(isInt(menuInput)) || std::stoi(menuInput) < 1 || std::stoi(menuInput) > 4) && !exit) { //Reprompt if not valid input
         std::cout << "Invalid input" << std::endl << "> ";
         std::getline(std::cin, menuInput);
         checkEmptyInput(menuInput);
         if (checkEOF(exit)) {} //Exit
      }

      if (!exit) {
         if (std::stoi(menuInput) == 1) {
            newGame(exit);
         } else if (std::stoi(menuInput) == 2) {
            loadGame(exit);
         } else if (std::stoi(menuInput) == 3) {
            credits();
         } else if (std::stoi(menuInput) == 4) {
            exit = true;
         }
      }
   }
}

void GameMenu::newGame(bool &exit) {
   bool newGame = true;
   // Keep displaying new game message until players, board are properly created or user quit the game
   while (newGame)
   {
      std::cout << std::endl << "Starting a New Game" << std::endl;
      // Only allow 2 players
      std::string playerNames[NUM_OF_PLAYERS];
      // Only allow uppercase
      std::regex regexPattern("[A-Z]+");

      int playerIndex = 1;
      bool isDuplicate = false;
      // Loop until 2 players name are inputted
      while (playerIndex <= NUM_OF_PLAYERS && newGame) {
         if (!isDuplicate) {
            std::cout << std::endl << "Enter a name for player " << playerIndex << " (uppercase characters only)" << std::endl << "> ";
         }
         isDuplicate = false;
         std::string playerName;
         std::getline(std::cin, playerName);
         if (checkEOF(exit)) { 
            newGame = false; // Exit
         } 
         else {
            while (!regex_match(playerName, regexPattern) && newGame) { //Reprompt if not valid input
               std::cout << "Invalid input" << std::endl << "> ";
               std::getline(std::cin, playerName);
               if (checkEOF(exit)) { 
                  newGame = false; // Exit
               } 
            }
            if (newGame) { //Valid input and no escape character
               for (int i = 0; i < NUM_OF_PLAYERS; i++) {
                  if (playerNames[i] == playerName) {
                     isDuplicate = true;
                  }
               }
               if (isDuplicate) {
                  std::cout << "Player name already exists" << std::endl << "> ";
               } else {
                  // Add player name to the array
                  playerNames[playerIndex-1] = playerName;
                  playerIndex++;
               }
            }
         }
      }
      //All player names have been created
      if (newGame) { 
         std::vector<Player*> players; 
         // Add every players to vector
         for (int i = 0; i < NUM_OF_PLAYERS; i++) { 
            players.push_back(new Player(playerNames[i]));
         }
         TileBag* tileBag = new TileBag(); 
         for (int i = 0; i < NUM_OF_PLAYERS; i++) { 
             // distribute a number of tiles to a player
            for (int j = 0; j < NUM_OF_TILES_IN_HAND; j++) {
                // Add tiles to a player hand
               players[i]->addToBackOfHand(new Tile(*(tileBag->getFront())));
               // remove the tiles added from the bag
               tileBag->deleteFront();
            }
         }
         // Create a board of size 26x26
         Board* board = new Board(BOARD_DIMENSION);
         // Start the game
         std::cout << std::endl << "Let's Play!" << std::endl << std::endl;
         playGame(players, tileBag, board, 0);
         // Terminate the loop
         newGame = false;
         exit = true;
      }
   }
}

void GameMenu::loadGame(bool &exit) {
   bool loadGame = true;
   std::ifstream loadFile;

   std::cout << std::endl << "Enter the filename from which to load a game" << std::endl << "> ";
   std::string fileName;
   // Input file name without .save
   std::getline(std::cin, fileName);
   loadFile.open(fileName + ".save");

   if (checkEOF(exit)) { 
      loadGame = false; // Exit
   } 
   else {
      while (!loadFile && loadGame) { // asking for reprompt until a valid filename is inputed
         std::cout << "Invalid input" << std::endl << "> ";
         std::getline(std::cin, fileName);
         loadFile.open(fileName + ".save");

         if (checkEOF(exit)) {
            loadGame = false; // Exit
         } 
      }
      if (loadGame) {
         if (isCorrectFormat(fileName)) { // load the game as a valid file is found
            std::cout << std::endl;
            std::vector<Player*> players;
            int numberOfPlayers = 0;
            bool isPlayer = true;

            std::string nextLine;
            getline(loadFile, nextLine);
            
            // Load every player info
            while (isPlayer) {
               players.push_back(new Player(nextLine)); //Player name

               getline(loadFile, nextLine); //Player score
               players[numberOfPlayers]->addScore(std::stoi(nextLine));

               getline(loadFile, nextLine); //Player hand
               std::vector<std::string> tiles = splitToTokens(nextLine, ',');
               for (long unsigned int i = 0; i < tiles.size(); i++) {
                  //Add the tiles to a player hand
                  players[numberOfPlayers]->addToBackOfHand(new Tile(tiles[i].at(0), std::stoi(tiles[i].substr(1))));
               }

               numberOfPlayers++;
               getline(loadFile, nextLine);
               if (!isAlphabetic(nextLine) && !isUpper(nextLine)) { //Every third line in the save file is a new player
                  isPlayer = false;
               } 
            }

            std::vector<std::string> boardSize = splitToTokens(nextLine, ',');
            Board* board = new Board(std::stoi(boardSize[0])); //Board dimension

            getline(loadFile, nextLine); //Board states
            std::vector<std::string> states = splitToTokens(nextLine, ',');
            // Place all the tiles on the board as in the loaded game
            for (long unsigned int i = 0; i < states.size(); i ++) {
               board->placeTileOnSquare(states[i].substr(0,2), states[i].substr(3));
            }

            getline(loadFile, nextLine); //Tile bag
            std::vector<std::string> tiles = splitToTokens(nextLine, ',');
            TileBag* tileBag = new TileBag(true);
            // Add all the available tiles to the tilebag as in the loaded game
            for (long unsigned int i = 0; i < tiles.size(); i++) {
               tileBag->addToBack(new Tile(tiles[i].at(0), std::stoi(tiles[i].substr(1))));
            }

            getline(loadFile, nextLine); //Current player
            int currentPlayerIndex = 0;
            // Jump to a player turn as in the loaded game
            for (int i = 0; i < numberOfPlayers; i++) {
               if (nextLine == players[i]->getName()) {
                  currentPlayerIndex = i;
               }
            }

            loadFile.close();
            std::cout << "Quirkle game successfully loaded" << std::endl << std::endl;
            // Start the game
            playGame(players, tileBag, board, currentPlayerIndex);
            exit = true;
         } else {
            std::cout << "Incorrect file format" << std::endl << std::endl;
         }
      }
   } 
}

void GameMenu::credits() {
   std::cout << std::endl << "-----------------------------------" << std::endl;
   std::cout << "Name: Chang Liu" << std::endl;
   std::cout << "Student ID: s3651885" << std::endl;
   std::cout << "Email: s3651885@student.rmit.edu.au" << std::endl << std::endl;
   std::cout << "Name: Quang Minh Duong" << std::endl;
   std::cout << "Student ID: s3759757" << std::endl;
   std::cout << "Email: s3759757@student.rmit.edu.au" << std::endl << std::endl;
   std::cout << "Name: Quoc Dat Tran" << std::endl;
   std::cout << "Student ID: s3827826" << std::endl;
   std::cout << "Email: s3827826@student.rmit.edu.au" << std::endl << std::endl;
   std::cout << "Name: Quy Dat Le" << std::endl;
   std::cout << "Student ID: s3794387" << std::endl;
   std::cout << "Email: s3794387@student.rmit.edu.au" << std::endl;
   std::cout << "-----------------------------------" << std::endl << std::endl;
}

void GameMenu::playGame(std::vector<Player*> players, TileBag* tileBag, Board* board, int playersTurnIndex) {
   bool playing = true;
   long unsigned int playerIndex = playersTurnIndex;
   // Keep the game running until the tilebag is empty or a player quit the game
   while (playing) {
      std::cout << players[playerIndex]->getName() << ", it's your turn" << std::endl;
      // Display score for each player
      for (long unsigned int i = 0; i < players.size(); i++) {
         std::cout << "Score for " << players[i]->getName() << ": " << players[i]->getScore() << std::endl;
      }
      // Output the board
      board->printBoard();
      // Display current player's tiles in the hand
      std::cout << "Your hand is" << std::endl << players[playerIndex]->getHandAsString() << std::endl << std::endl;
        // Ask current player to play their turn
      playersTurn(players[playerIndex], board, tileBag, playing, players);
   
      playerIndex++;
      // reset the playerindex if it reach 2 
      if (playerIndex == players.size()) {
         playerIndex = 0; 
      }
   }
   //memory management here
   for (long unsigned int i = 0; i < players.size(); i++) {
      delete players[i];
   }
   delete tileBag;
   delete board;
}

void GameMenu::playersTurn(Player* player, Board* board, TileBag* tileBag, bool &playing, std::vector<Player*> players) {
   bool playersTurn = true;
   
   // Check if there are any valid moves left while tilebag it empty
   if (tileBag->isEmpty()) {
      bool validMove = false;
      for (int i = 0; i < player->handSize(); i++) {
         if (board->checkAllValidMoves(player->getTile(i)->getColour()+std::to_string(player->getTile(i)->getShape()))) {
            validMove = true;
         }
         board->resetPoints();
      }
      if (!validMove) { // End the game
         std::cout << "No valid moves left" << std::endl;
         gameOver(players);
         playing = false;
         playersTurn = false;
         std::cout << std::endl;
      }
   }
   // Keep asking for a player to input their turn command until it is valid
   while (playersTurn) {
      std::string input;
      std::cout << "> ";
      std::getline(std::cin, input);
      std::vector<std::string> tokens = splitToTokens(input, ' ');
      // Place a tile on a board's coordinate if the input is valid (command length =4,first word is place, third word is at,
      // a tile is inputed in right format and is exist in the player's hand, a board coordinate is also in right format)
      if (tokens.size() == 4 && tokens[0] == "place" && tokens[2] == "at" && checkTileFormat(tokens[1]) && checkGridFormat(tokens[3])) {
        // Only allow player to place a tile on a coordinate that is exist in the board
         if (player->tileInHand(tokens[1]) && board->isValidMove(tokens[1], tokens[3]))
         {
             //remove the called tile from the player hand
            player->deleteTile(tokens[1]);
            // place it on the board
            board->placeTileOnSquare(tokens[1], tokens[3]);
            // update score
            player->addScore(board->getPoints());
            board->resetPoints();
            // run quirkle special op
            if (board->isQuirkle()) {
               std::cout << std::endl << "QWIRKLE!!!" << std::endl;
               board->resetQuirkle();
            }
            // Game is over
            if (tileBag->isEmpty() && player->handIsEmpty()) {
               gameOver(players);
               // end the game
               playing = false;
               playersTurn = false;
            } else if (!tileBag->isEmpty()) {
               // add new tile to the player hand if there are still tiles
               player->addToBackOfHand(new Tile(*(tileBag->getFront())));
               // remove the tile that has been added to the player from the bag
               tileBag->deleteFront();
            } 
            // terminate the loop
            playersTurn = false;
            std::cout << std::endl;
         }
         else {
            std::cout << "Invalid input" << std::endl;
            board->resetPoints();
         }
      }
      // change a tile in a player hand with another tile in the bag if the input is valid
      // (command length = 2, first word is replace, second word is a valid tile that exist in the player's hand )
      else if (tokens.size() == 2 && tokens[0] == "replace" && checkTileFormat(tokens[1])) {
         if (player->tileInHand(tokens[1])) {
            if (tileBag->isEmpty()) {
               std::cout << "Tile bag is empty" << std::endl;
            } else {
               player->addToBackOfHand(new Tile(*(tileBag->getFront())));
               tileBag->deleteFront();
               tileBag->addToBack(new Tile(*(player->getTile(tokens[1]))));
               player->deleteTile(tokens[1]);
               playersTurn = false;
               std::cout << std::endl;
            }
         }
         else {
            std::cout << "Invalid input" << std::endl;
         }
      }
      // save the game if the input is valid (first word is save, second word is a file that player want to save to)
      else if (tokens.size() == 2 && tokens[0] == "save") {
          // add file extension
         std::ofstream saveFile(tokens[1] + ".save", std::ios::trunc);
         // add every player's name, score, tile in their hand to the file
         for (int i = 0; i < NUM_OF_PLAYERS; i++) {
            saveFile << players[i]->getName() << std::endl;
            saveFile << players[i]->getScore() << std::endl;
            saveFile << players[i]->getHandAsString() << std::endl;
         } 
         // add board dimension to the file
         saveFile << std::to_string(board->getBoardDimension()) + "," + std::to_string(board->getBoardDimension()) << std::endl;
         // add location of tiles and the tiles themselves that are placed on the board to the file
         saveFile << board->getBoardStateAsString() << std::endl;
         // add the remaining tile from the bag to the file
         saveFile << tileBag->getTileBagAsString() << std::endl;
         saveFile << player->getName() << std::endl;
         saveFile.close();
         std::cout << "Game successfully saved" << std::endl << std::endl;
      }
      // Quit the game if player choose input quit or escape character
      else if ((tokens.size() == 1 && tokens[0] == "quit") || std::cin.eof()) {
         std::cout << std::endl;
         playersTurn = false;
         playing = false;
      }
      else 
         std::cout << "Invalid input" << std::endl;
   }
}

//Helpers

void GameMenu::gameOver(std::vector<Player*> players) {
   std::cout << std::endl << "Game over" << std::endl;
   int highestScore = 0;
   std::string highestPlayer;
   for (long unsigned int i = 0; i < players.size(); i++) {
      if (players[i]->getScore() > highestScore) {
         highestScore = players[i]->getScore();
         highestPlayer = players[i]->getName();
      } else if (players[i]->getScore() == highestScore) {
         highestPlayer = "DRAW!";
      }
      std::cout << "Score for " << players[i]->getName() <<": " << players[i]->getScore() << std::endl;
   }
   if (highestPlayer == "DRAW!") {
      std::cout << highestPlayer;
   } else {
      std::cout << "Player " << highestPlayer <<" won!";
   }
}

bool GameMenu::isInt(std::string string) {
   bool isInt = true;

   for (long long unsigned int i = 0; i < string.length(); i++) {
      if (isdigit(string[i]) == false)
         isInt = false;
   }
   return isInt;
}

bool GameMenu::isAlphabetic(std::string string) {
   bool isAlphabetic = true;
   for (long unsigned int i = 0; i < string.size(); i++){
      if (!isalpha(string[i])) {
         isAlphabetic = false;
      }
   }
   return isAlphabetic;
}

bool GameMenu::isUpper(std::string string) {
   bool isUpper = true;
   for (long unsigned int i = 0; i < string.size(); i++){
      if (!isupper(string[i])) {
         isUpper = false;
      }
   }
   return isUpper;
}

std::vector<std::string> GameMenu::splitToTokens(std::string string, char delimiter) {
   //Reference: https://www.geeksforgeeks.org/tokenizing-a-string-cpp/
   std::vector <std::string> tokens;
   std::stringstream ss(string);
   std::string intermediate;
    
   while(getline(ss, intermediate, delimiter))
      tokens.push_back(intermediate);
      
   return tokens;
}

bool GameMenu::isCorrectFormat(std::string fileName) {
   std::ifstream loadFile;
   loadFile.open(fileName + ".save");
   std::string nextLine;
   getline(loadFile, nextLine); // This line should be a player name

   bool isCorrectFormat = true;
   bool isPlayer = true;

   if (loadFile.eof()) { // If the file is empty,
      isCorrectFormat = false;
   } else {
      std::vector<std::string> playerNames;
      while (isPlayer) {
         if (!isAlphabetic(nextLine) || !isUpper(nextLine)) { // Check player name format
            isCorrectFormat = false;
         // Only get next line while format is correct. This also stops null error if the file doesn't have enough lines
         } else { 
            playerNames.push_back(nextLine); // Store player name to check current player exists later on
            getline(loadFile, nextLine); // Player score
         }
         if (!isInt(nextLine)) { // Check score format
            isCorrectFormat = false;
         // Only get next line while format is correct. This also stops null error if the file doesn't have enough lines
         } else {
            getline(loadFile, nextLine); // Player hand
         }
         std::vector<std::string> tiles = splitToTokens(nextLine, ',');
         if (tiles.size() == 0 || tiles.size() > NUM_OF_TILES_IN_HAND) { // Too many tiles
            isCorrectFormat = false;
         }
         for (long unsigned int i = 0; i < tiles.size(); i++) { // Check hand format
            if (!checkTileFormat(tiles[i])) {
               isCorrectFormat = false;
            }
         }
         // Only get next line while format is correct. This also stops null error if the file doesn't have enough lines
         if (isCorrectFormat) {
            getline(loadFile, nextLine); // This line is either another player or the board size
         }
         if (!isAlphabetic(nextLine) || !isUpper(nextLine)) { // Check if there are more players
            isPlayer = false;
         } 
      }
      std::vector<std::string> boardSize = splitToTokens(nextLine, ',');
      if (!isInt(boardSize[0]) && boardSize[0] == boardSize[1]) { // Check board size format
         isCorrectFormat = false;
      // Only get next line while format is correct. This also stops null error if the file doesn't have enough lines
      } else {
         getline(loadFile, nextLine); // Board state
      }
      std::vector<std::string> states = splitToTokens(nextLine, ',');
      for (long unsigned int i = 0; i < states.size(); i ++) {
         if (!checkTileFormat(states[i].substr(0,2)) || !checkGridFormat(states[i].substr(3))){ 
            isCorrectFormat = false;
         }
      }
      // Only get next line while format is correct. This also stops null error if the file doesn't have enough lines
      if (isCorrectFormat) {
         getline(loadFile, nextLine); // Tiles in tile bag
      }
      std::vector<std::string> tiles = splitToTokens(nextLine, ',');
      if (tiles.size() > (TILEBAG_SIZE)-(2*NUM_OF_TILES_IN_HAND)) {
         isCorrectFormat = false;
      }
      for (long unsigned int i = 0; i < tiles.size(); i++) { // Check tile bag format
         if (!checkTileFormat(tiles[i])) {
            isCorrectFormat = false;
         }
      }
      // Only get next line while format is correct. This also stops null error if the file doesn't have enough lines
      if (isCorrectFormat) {
         getline(loadFile, nextLine); // Current player
      }
      bool playerExists = false;
      for (long unsigned int i = 0; i < playerNames.size(); i++) {
         if (playerNames[i] == nextLine) {
            playerExists = true;
         }
      }
      if (!playerExists) { // If player exists then the format is also correct
         isCorrectFormat = false;
      }

      loadFile.close();
   }

   return isCorrectFormat;
}

bool GameMenu::checkTileFormat(std::string tile) {
   bool isValid = false;
   if ((tile.substr(0,1) == "R" || tile.substr(0,1) == "O" || tile.substr(0,1) == "Y" ||
       tile.substr(0,1) == "G" || tile.substr(0,1) == "B" || tile.substr(0,1) == "P") &&
       isInt(tile.substr(1))) {
         if ((std::stoi(tile.substr(1)) >= 1 && std::stoi(tile.substr(1)) <= 6)) {
            isValid = true;
         }
   }
   return isValid;
}

bool GameMenu::checkGridFormat(std::string grid) {
   bool isValid = false;
   if (isUpper(grid.substr(0,1)) && isAlphabetic(grid.substr(0,1)) && isInt(grid.substr(1))) {
      if (std::stoi(grid.substr(1)) >= 0 && std::stoi(grid.substr(1)) < BOARD_DIMENSION) {
         isValid = true;
      }
   }
   return isValid;
}

bool GameMenu::checkEOF(bool &exit) {
   if (std::cin.eof()) { //Check input for escape character
      std::cout << std::endl;
      exit = true;
   }
   return exit;
}

void GameMenu::checkEmptyInput(std::string &input) {
   if (input.empty()) {
      input = " ";
   }
}