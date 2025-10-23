#include "Player.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <cctype>

// Player base class
Player::Player(Color c, const std::string& n) : color(c), name(n) {}

// HumanPlayer implementation
HumanPlayer::HumanPlayer(Color c, const std::string& n) : Player(c, n) {}

Position HumanPlayer::parsePosition(const std::string& pos) {
    if (pos.length() != 2) {
        return Position(-1, -1); // Invalid position
    }
    
    char col = std::tolower(pos[0]);
    char row = pos[1];
    
    if (col < 'a' || col > 'h' || row < '1' || row > '8') {
        return Position(-1, -1); // Invalid position
    }
    
    // Convert algebraic notation to array indices
    // a-h -> 0-7 (columns)
    // 8-1 -> 0-7 (rows)
    int colIndex = col - 'a';
    int rowIndex = 8 - (row - '0');
    
    return Position(rowIndex, colIndex);
}

std::pair<Position, Position> HumanPlayer::getMove(const Board& board) {
    std::string fromStr, toStr;
    
    while (true) {
        std::cout << name << "'s turn. Enter move (e.g., e2 e4): ";
        std::cin >> fromStr >> toStr;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please try again.\n";
            continue;
        }
        
        Position from = parsePosition(fromStr);
        Position to = parsePosition(toStr);
        
        if (!from.isValid() || !to.isValid()) {
            std::cout << "Invalid position. Please use algebraic notation (e.g., e2 e4).\n";
            continue;
        }
        
        Piece* piece = board.getPiece(from);
        if (!piece) {
            std::cout << "No piece at " << fromStr << ". Try again.\n";
            continue;
        }
        
        if (piece->getColor() != color) {
            std::cout << "That's not your piece! Try again.\n";
            continue;
        }
        
        if (!board.isMoveLegal(from, to, color)) {
            std::cout << "Illegal move. Try again.\n";
            continue;
        }
        
        return {from, to};
    }
}

// AIPlayer implementation
AIPlayer::AIPlayer(Color c, const std::string& n, int depth) 
    : Player(c, n), searchDepth(depth) {}

int AIPlayer::evaluateBoard(const Board& board) const {
    int score = 0;
    
    // Material evaluation
    score = board.evaluatePosition();
    
    // Adjust score based on perspective (AI is typically black)
    if (color == Color::BLACK) {
        score = -score;
    }
    
    return score;
}

int AIPlayer::minimax(Board& board, int depth, int alpha, int beta, bool isMaximizing) {
    // Check for terminal states
    Color currentColor = isMaximizing ? color : (color == Color::WHITE ? Color::BLACK : Color::WHITE);
    
    if (depth == 0) {
        return evaluateBoard(board);
    }
    
    if (board.isCheckmate(currentColor)) {
        return isMaximizing ? -100000 : 100000;
    }
    
    if (board.isStalemate(currentColor)) {
        return 0; // Draw
    }
    
    std::vector<std::pair<Position, Position>> allMoves = board.getAllValidMoves(currentColor);
    
    if (allMoves.empty()) {
        return isMaximizing ? -100000 : 100000;
    }
    
    if (isMaximizing) {
        int maxEval = std::numeric_limits<int>::min();
        
        for (const auto& move : allMoves) {
            // Create a copy of the board
            auto boardCopy = board.clone();
            boardCopy->makeMove(move.first, move.second);
            
            int eval = minimax(*boardCopy, depth - 1, alpha, beta, false);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            
            if (beta <= alpha) {
                break; // Alpha-beta pruning
            }
        }
        
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        
        for (const auto& move : allMoves) {
            // Create a copy of the board
            auto boardCopy = board.clone();
            boardCopy->makeMove(move.first, move.second);
            
            int eval = minimax(*boardCopy, depth - 1, alpha, beta, true);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            
            if (beta <= alpha) {
                break; // Alpha-beta pruning
            }
        }
        
        return minEval;
    }
}

std::pair<Position, Position> AIPlayer::findBestMove(Board& board) {
    std::vector<std::pair<Position, Position>> allMoves = board.getAllValidMoves(color);
    
    if (allMoves.empty()) {
        return {Position(-1, -1), Position(-1, -1)}; // No valid moves
    }
    
    int bestValue = std::numeric_limits<int>::min();
    std::pair<Position, Position> bestMove = allMoves[0];
    
    std::cout << "AI is thinking";
    std::cout.flush();
    
    for (const auto& move : allMoves) {
        auto boardCopy = board.clone();
        boardCopy->makeMove(move.first, move.second);
        
        int moveValue = minimax(*boardCopy, searchDepth - 1, 
                               std::numeric_limits<int>::min(), 
                               std::numeric_limits<int>::max(), 
                               false);
        
        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = move;
        }
        
        std::cout << ".";
        std::cout.flush();
    }
    
    std::cout << " Done!\n";
    
    return bestMove;
}

std::pair<Position, Position> AIPlayer::getMove(const Board& board) {
    Board boardCopy = board; // Create a non-const copy
    auto move = findBestMove(boardCopy);
    
    // Convert positions to algebraic notation for display
    char fromCol = 'a' + move.first.col;
    char fromRow = '8' - move.first.row;
    char toCol = 'a' + move.second.col;
    char toRow = '8' - move.second.row;
    
    std::cout << name << " moves: " << fromCol << fromRow << " " << toCol << toRow << "\n";
    
    return move;
}
