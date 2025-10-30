#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Player.h"
#include <memory>

enum class GameState {
    PLAYING,
    CHECKMATE,
    STALEMATE,
    DRAW
};

class Game {
private:
    Board board;
    std::unique_ptr<Player> whitePlayer;
    std::unique_ptr<Player> blackPlayer;
    Player* currentPlayer;
    GameState gameState;
    int moveCount;

public:
    Game();
    
    // Initialize the game
    void initialize(bool playAgainstAI = true);
    
    // Main game loop
    void play();
    
    // Process a single turn
    bool processTurn();
    
    // Check game state (checkmate, stalemate, etc.)
    void updateGameState();
    
    // Display game information
    void displayGameInfo() const;
    
    // Get current player
    Player* getCurrentPlayer() const { return currentPlayer; }
    
    // Switch to next player
    void switchPlayer();
    
    // Display winner
    void displayResult() const;

private:
    // Helper function to display welcome message
    void displayWelcome() const;
};

#endif // GAME_H
