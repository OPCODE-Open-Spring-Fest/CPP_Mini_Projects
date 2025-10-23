#include "Board.h"
#include <iostream>
#include <algorithm>

Board::Board() {
    // Initialize empty board
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = nullptr;
        }
    }
}

void Board::initialize() {
    // Clear board
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = nullptr;
        }
    }
    
    // Place black pieces (row 0-1)
    board[0][0] = std::make_unique<Rook>(Color::BLACK, Position(0, 0));
    board[0][1] = std::make_unique<Knight>(Color::BLACK, Position(0, 1));
    board[0][2] = std::make_unique<Bishop>(Color::BLACK, Position(0, 2));
    board[0][3] = std::make_unique<Queen>(Color::BLACK, Position(0, 3));
    board[0][4] = std::make_unique<King>(Color::BLACK, Position(0, 4));
    board[0][5] = std::make_unique<Bishop>(Color::BLACK, Position(0, 5));
    board[0][6] = std::make_unique<Knight>(Color::BLACK, Position(0, 6));
    board[0][7] = std::make_unique<Rook>(Color::BLACK, Position(0, 7));
    
    for (int col = 0; col < 8; col++) {
        board[1][col] = std::make_unique<Pawn>(Color::BLACK, Position(1, col));
    }
    
    // Place white pieces (row 6-7)
    for (int col = 0; col < 8; col++) {
        board[6][col] = std::make_unique<Pawn>(Color::WHITE, Position(6, col));
    }
    
    board[7][0] = std::make_unique<Rook>(Color::WHITE, Position(7, 0));
    board[7][1] = std::make_unique<Knight>(Color::WHITE, Position(7, 1));
    board[7][2] = std::make_unique<Bishop>(Color::WHITE, Position(7, 2));
    board[7][3] = std::make_unique<Queen>(Color::WHITE, Position(7, 3));
    board[7][4] = std::make_unique<King>(Color::WHITE, Position(7, 4));
    board[7][5] = std::make_unique<Bishop>(Color::WHITE, Position(7, 5));
    board[7][6] = std::make_unique<Knight>(Color::WHITE, Position(7, 6));
    board[7][7] = std::make_unique<Rook>(Color::WHITE, Position(7, 7));
    
    // Set king positions
    whiteKingPos = Position(7, 4);
    blackKingPos = Position(0, 4);
}

Piece* Board::getPiece(const Position& pos) const {
    if (!pos.isValid()) return nullptr;
    return board[pos.row][pos.col].get();
}

bool Board::isEmpty(const Position& pos) const {
    if (!pos.isValid()) return false;
    return board[pos.row][pos.col] == nullptr;
}

bool Board::hasEnemyPiece(const Position& pos, Color playerColor) const {
    if (!pos.isValid()) return false;
    Piece* piece = board[pos.row][pos.col].get();
    return piece != nullptr && piece->getColor() != playerColor;
}

bool Board::makeMove(const Position& from, const Position& to) {
    if (!from.isValid() || !to.isValid()) return false;
    
    Piece* piece = getPiece(from);
    if (!piece) return false;
    
    // Check if move is legal
    if (!isMoveLegal(from, to, piece->getColor())) {
        return false;
    }
    
    // Capture enemy piece if present
    if (getPiece(to)) {
        board[to.row][to.col] = nullptr;
    }
    
    // Move the piece
    board[to.row][to.col] = std::move(board[from.row][from.col]);
    board[to.row][to.col]->setPosition(to);
    board[to.row][to.col]->setHasMoved(true);
    
    // Update king position if king moved
    if (board[to.row][to.col]->getType() == PieceType::KING) {
        if (board[to.row][to.col]->getColor() == Color::WHITE) {
            whiteKingPos = to;
        } else {
            blackKingPos = to;
        }
    }
    
    return true;
}

bool Board::isKingInCheck(Color kingColor) const {
    Position kingPos = (kingColor == Color::WHITE) ? whiteKingPos : blackKingPos;
    Color enemyColor = (kingColor == Color::WHITE) ? Color::BLACK : Color::WHITE;
    
    return isPositionUnderAttack(kingPos, enemyColor);
}

bool Board::isPositionUnderAttack(const Position& pos, Color attackerColor) const {
    // Check all pieces of the attacking color
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece* piece = board[row][col].get();
            if (piece && piece->getColor() == attackerColor) {
                std::vector<Position> moves = piece->getValidMoves(*this);
                for (const Position& move : moves) {
                    if (move == pos) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Board::wouldKingBeSafe(const Position& from, const Position& to, Color playerColor) const {
    // Create a temporary board state
    Board tempBoard;
    
    // Copy current board state
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col]) {
                tempBoard.board[row][col] = board[row][col]->clone();
            }
        }
    }
    tempBoard.whiteKingPos = whiteKingPos;
    tempBoard.blackKingPos = blackKingPos;
    
    // Make the move on temporary board
    if (tempBoard.board[to.row][to.col]) {
        tempBoard.board[to.row][to.col] = nullptr;
    }
    tempBoard.board[to.row][to.col] = std::move(tempBoard.board[from.row][from.col]);
    tempBoard.board[to.row][to.col]->setPosition(to);
    
    // Update king position if king moved
    if (tempBoard.board[to.row][to.col]->getType() == PieceType::KING) {
        if (playerColor == Color::WHITE) {
            tempBoard.whiteKingPos = to;
        } else {
            tempBoard.blackKingPos = to;
        }
    }
    
    // Check if king is in check after the move
    return !tempBoard.isKingInCheck(playerColor);
}

bool Board::isMoveLegal(const Position& from, const Position& to, Color playerColor) const {
    Piece* piece = getPiece(from);
    if (!piece || piece->getColor() != playerColor) return false;
    
    // Check if the move is in the piece's valid moves
    std::vector<Position> validMoves = piece->getValidMoves(*this);
    bool isValidMove = false;
    for (const Position& move : validMoves) {
        if (move == to) {
            isValidMove = true;
            break;
        }
    }
    
    if (!isValidMove) return false;
    
    // Check if the move would leave the king in check
    return wouldKingBeSafe(from, to, playerColor);
}

std::vector<std::pair<Position, Position>> Board::getAllValidMoves(Color playerColor) const {
    std::vector<std::pair<Position, Position>> allMoves;
    
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece* piece = board[row][col].get();
            if (piece && piece->getColor() == playerColor) {
                Position from(row, col);
                std::vector<Position> validMoves = piece->getValidMoves(*this);
                
                for (const Position& to : validMoves) {
                    if (isMoveLegal(from, to, playerColor)) {
                        allMoves.push_back({from, to});
                    }
                }
            }
        }
    }
    
    return allMoves;
}

bool Board::isCheckmate(Color playerColor) const {
    // If not in check, it's not checkmate
    if (!isKingInCheck(playerColor)) return false;
    
    // If there are any valid moves, it's not checkmate
    return getAllValidMoves(playerColor).empty();
}

bool Board::isStalemate(Color playerColor) const {
    // If in check, it's not stalemate
    if (isKingInCheck(playerColor)) return false;
    
    // If there are no valid moves, it's stalemate
    return getAllValidMoves(playerColor).empty();
}

Position Board::getKingPosition(Color kingColor) const {
    return (kingColor == Color::WHITE) ? whiteKingPos : blackKingPos;
}

int Board::evaluatePosition() const {
    int score = 0;
    
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece* piece = board[row][col].get();
            if (piece) {
                int pieceValue = piece->getValue();
                if (piece->getColor() == Color::WHITE) {
                    score += pieceValue;
                } else {
                    score -= pieceValue;
                }
            }
        }
    }
    
    return score;
}

void Board::display() const {
    std::cout << "\n    a   b   c   d   e   f   g   h\n";
    std::cout << "  +---+---+---+---+---+---+---+---+\n";
    
    for (int row = 0; row < 8; row++) {
        std::cout << 8 - row << " |";
        for (int col = 0; col < 8; col++) {
            Piece* piece = board[row][col].get();
            if (piece) {
                std::cout << " " << piece->getSymbol() << " |";
            } else {
                std::cout << "   |";
            }
        }
        std::cout << " " << 8 - row << "\n";
        std::cout << "  +---+---+---+---+---+---+---+---+\n";
    }
    
    std::cout << "    a   b   c   d   e   f   g   h\n\n";
}

void Board::placePiece(std::unique_ptr<Piece> piece, const Position& pos) {
    if (pos.isValid()) {
        board[pos.row][pos.col] = std::move(piece);
    }
}

std::unique_ptr<Board> Board::clone() const {
    auto clonedBoard = std::make_unique<Board>();
    
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col]) {
                clonedBoard->board[row][col] = board[row][col]->clone();
            }
        }
    }
    
    clonedBoard->whiteKingPos = whiteKingPos;
    clonedBoard->blackKingPos = blackKingPos;
    
    return clonedBoard;
}
