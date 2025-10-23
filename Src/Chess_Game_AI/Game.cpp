#include "Game.h"
#include <iostream>
#include <limits>

Game::Game() : currentPlayer(nullptr), gameState(GameState::PLAYING), moveCount(0) {}

void Game::displayWelcome() const {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "      CHESS GAME WITH AI IN C++        \n";
    std::cout << "========================================\n";
    std::cout << "\n";
    std::cout << "Piece Symbols:\n";
    std::cout << "  White: K=King, Q=Queen, R=Rook, B=Bishop, N=Knight, P=Pawn\n";
    std::cout << "  Black: k=king, q=queen, r=rook, b=bishop, n=knight, p=pawn\n";
    std::cout << "\n";
    std::cout << "How to play:\n";
    std::cout << "  - Enter moves using algebraic notation (e.g., e2 e4)\n";
    std::cout << "  - Columns: a-h, Rows: 1-8\n";
    std::cout << "  - White starts first\n";
    std::cout << "\n";
    std::cout << "========================================\n\n";
}

void Game::initialize(bool playAgainstAI) {
    displayWelcome();
    
    // Initialize the board
    board.initialize();
    
    // Create players
    whitePlayer = std::make_unique<HumanPlayer>(Color::WHITE, "White Player");
    
    if (playAgainstAI) {
        std::cout << "Select AI difficulty:\n";
        std::cout << "1. Easy (depth 1)\n";
        std::cout << "2. Medium (depth 2)\n";
        std::cout << "3. Hard (depth 3)\n";
        std::cout << "Enter choice (1-3): ";
        
        int choice;
        std::cin >> choice;
        
        if (std::cin.fail() || choice < 1 || choice > 3) {
            std::cout << "Invalid choice. Setting to Medium difficulty.\n";
            choice = 2;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        
        blackPlayer = std::make_unique<AIPlayer>(Color::BLACK, "AI", choice);
    } else {
        blackPlayer = std::make_unique<HumanPlayer>(Color::BLACK, "Black Player");
    }
    
    // White starts first
    currentPlayer = whitePlayer.get();
    gameState = GameState::PLAYING;
    moveCount = 0;
    
    std::cout << "\nGame started! White moves first.\n\n";
}

void Game::displayGameInfo() const {
    std::cout << "\n";
    std::cout << "Move #" << (moveCount / 2 + 1);
    std::cout << " | Current turn: " << currentPlayer->getName();
    
    Color currentColor = currentPlayer->getColor();
    if (board.isKingInCheck(currentColor)) {
        std::cout << " - CHECK!";
    }
    std::cout << "\n";
}

void Game::updateGameState() {
    Color currentColor = currentPlayer->getColor();
    
    if (board.isCheckmate(currentColor)) {
        gameState = GameState::CHECKMATE;
    } else if (board.isStalemate(currentColor)) {
        gameState = GameState::STALEMATE;
    }
}

void Game::switchPlayer() {
    if (currentPlayer == whitePlayer.get()) {
        currentPlayer = blackPlayer.get();
    } else {
        currentPlayer = whitePlayer.get();
    }
}

bool Game::processTurn() {
    // Display current game state
    displayGameInfo();
    board.display();
    
    // Get move from current player
    auto move = currentPlayer->getMove(board);
    
    // Make the move
    if (board.makeMove(move.first, move.second)) {
        moveCount++;
        
        // Check game state after move
        switchPlayer();
        updateGameState();
        
        return true;
    } else {
        std::cout << "Invalid move! Try again.\n";
        return false;
    }
}

void Game::displayResult() const {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "              GAME OVER                 \n";
    std::cout << "========================================\n";
    
    board.display();
    
    switch (gameState) {
        case GameState::CHECKMATE:
            std::cout << "\nCheckmate! ";
            if (currentPlayer == whitePlayer.get()) {
                std::cout << blackPlayer->getName() << " wins!\n";
            } else {
                std::cout << whitePlayer->getName() << " wins!\n";
            }
            break;
            
        case GameState::STALEMATE:
            std::cout << "\nStalemate! The game is a draw.\n";
            break;
            
        case GameState::DRAW:
            std::cout << "\nThe game is a draw.\n";
            break;
            
        default:
            break;
    }
    
    std::cout << "\nTotal moves: " << moveCount << "\n";
    std::cout << "========================================\n\n";
}

void Game::play() {
    while (gameState == GameState::PLAYING) {
        processTurn();
    }
    
    displayResult();
}
