#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include <array>
#include <memory>
#include <vector>

struct Move {
    Position from;
    Position to;
    std::unique_ptr<Piece> capturedPiece;
    bool isCastling;
    bool isEnPassant;
    
    Move(Position f, Position t) 
        : from(f), to(t), capturedPiece(nullptr), isCastling(false), isEnPassant(false) {}
};

class Board {
private:
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> board;
    std::vector<Move> moveHistory;
    Position whiteKingPos;
    Position blackKingPos;

public:
    Board();
    
    // Initialize the board with standard chess setup
    void initialize();
    
    // Get piece at position
    Piece* getPiece(const Position& pos) const;
    
    // Check if position is empty
    bool isEmpty(const Position& pos) const;
    
    // Check if position has enemy piece
    bool hasEnemyPiece(const Position& pos, Color playerColor) const;
    
    // Make a move
    bool makeMove(const Position& from, const Position& to);
    
    // Undo last move
    void undoMove();
    
    // Check if king is in check
    bool isKingInCheck(Color kingColor) const;
    
    // Check if it's checkmate
    bool isCheckmate(Color playerColor) const;
    
    // Check if it's stalemate
    bool isStalemate(Color playerColor) const;
    
    // Get all valid moves for a player
    std::vector<std::pair<Position, Position>> getAllValidMoves(Color playerColor) const;
    
    // Validate if a move is legal (doesn't leave king in check)
    bool isMoveLegal(const Position& from, const Position& to, Color playerColor) const;
    
    // Display the board
    void display() const;
    
    // Get king position
    Position getKingPosition(Color kingColor) const;
    
    // Evaluate board position for AI
    int evaluatePosition() const;
    
    // Clone board for simulation
    std::unique_ptr<Board> clone() const;

private:
    // Helper function to check if position is under attack
    bool isPositionUnderAttack(const Position& pos, Color attackerColor) const;
    
    // Helper to simulate a move and check if king is safe
    bool wouldKingBeSafe(const Position& from, const Position& to, Color playerColor) const;
    
    // Place a piece on the board
    void placePiece(std::unique_ptr<Piece> piece, const Position& pos);
};

#endif // BOARD_H
