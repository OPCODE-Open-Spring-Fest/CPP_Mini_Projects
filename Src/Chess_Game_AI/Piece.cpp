#include "Piece.h"
#include "Board.h"

// Base Piece constructor
Piece::Piece(Color c, PieceType t, Position pos) 
    : color(c), type(t), position(pos), hasMoved(false) {}

// King implementation
King::King(Color c, Position pos) : Piece(c, PieceType::KING, pos) {}

std::vector<Position> King::getValidMoves(const Board& board) const {
    std::vector<Position> moves;
    
    // King can move one square in any direction
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };
    
    for (auto& dir : directions) {
        Position newPos(position.row + dir[0], position.col + dir[1]);
        if (newPos.isValid() && 
            (board.isEmpty(newPos) || board.hasEnemyPiece(newPos, color))) {
            moves.push_back(newPos);
        }
    }
    
    return moves;
}

std::unique_ptr<Piece> King::clone() const {
    auto cloned = std::make_unique<King>(color, position);
    cloned->setHasMoved(hasMoved);
    return cloned;
}

// Queen implementation
Queen::Queen(Color c, Position pos) : Piece(c, PieceType::QUEEN, pos) {}

std::vector<Position> Queen::getValidMoves(const Board& board) const {
    std::vector<Position> moves;
    
    // Queen moves like rook + bishop (all 8 directions)
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };
    
    for (auto& dir : directions) {
        for (int i = 1; i < 8; i++) {
            Position newPos(position.row + dir[0] * i, position.col + dir[1] * i);
            
            if (!newPos.isValid()) break;
            
            if (board.isEmpty(newPos)) {
                moves.push_back(newPos);
            } else {
                if (board.hasEnemyPiece(newPos, color)) {
                    moves.push_back(newPos);
                }
                break;
            }
        }
    }
    
    return moves;
}

std::unique_ptr<Piece> Queen::clone() const {
    auto cloned = std::make_unique<Queen>(color, position);
    cloned->setHasMoved(hasMoved);
    return cloned;
}

// Rook implementation
Rook::Rook(Color c, Position pos) : Piece(c, PieceType::ROOK, pos) {}

std::vector<Position> Rook::getValidMoves(const Board& board) const {
    std::vector<Position> moves;
    
    // Rook moves horizontally and vertically
    int directions[4][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    
    for (auto& dir : directions) {
        for (int i = 1; i < 8; i++) {
            Position newPos(position.row + dir[0] * i, position.col + dir[1] * i);
            
            if (!newPos.isValid()) break;
            
            if (board.isEmpty(newPos)) {
                moves.push_back(newPos);
            } else {
                if (board.hasEnemyPiece(newPos, color)) {
                    moves.push_back(newPos);
                }
                break;
            }
        }
    }
    
    return moves;
}

std::unique_ptr<Piece> Rook::clone() const {
    auto cloned = std::make_unique<Rook>(color, position);
    cloned->setHasMoved(hasMoved);
    return cloned;
}

// Bishop implementation
Bishop::Bishop(Color c, Position pos) : Piece(c, PieceType::BISHOP, pos) {}

std::vector<Position> Bishop::getValidMoves(const Board& board) const {
    std::vector<Position> moves;
    
    // Bishop moves diagonally
    int directions[4][2] = {
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };
    
    for (auto& dir : directions) {
        for (int i = 1; i < 8; i++) {
            Position newPos(position.row + dir[0] * i, position.col + dir[1] * i);
            
            if (!newPos.isValid()) break;
            
            if (board.isEmpty(newPos)) {
                moves.push_back(newPos);
            } else {
                if (board.hasEnemyPiece(newPos, color)) {
                    moves.push_back(newPos);
                }
                break;
            }
        }
    }
    
    return moves;
}

std::unique_ptr<Piece> Bishop::clone() const {
    auto cloned = std::make_unique<Bishop>(color, position);
    cloned->setHasMoved(hasMoved);
    return cloned;
}

// Knight implementation
Knight::Knight(Color c, Position pos) : Piece(c, PieceType::KNIGHT, pos) {}

std::vector<Position> Knight::getValidMoves(const Board& board) const {
    std::vector<Position> moves;
    
    // Knight moves in L-shape
    int knightMoves[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2},  {1, 2},  {2, -1},  {2, 1}
    };
    
    for (auto& move : knightMoves) {
        Position newPos(position.row + move[0], position.col + move[1]);
        
        if (newPos.isValid() && 
            (board.isEmpty(newPos) || board.hasEnemyPiece(newPos, color))) {
            moves.push_back(newPos);
        }
    }
    
    return moves;
}

std::unique_ptr<Piece> Knight::clone() const {
    auto cloned = std::make_unique<Knight>(color, position);
    cloned->setHasMoved(hasMoved);
    return cloned;
}

// Pawn implementation
Pawn::Pawn(Color c, Position pos) : Piece(c, PieceType::PAWN, pos) {}

std::vector<Position> Pawn::getValidMoves(const Board& board) const {
    std::vector<Position> moves;
    
    int direction = (color == Color::WHITE) ? -1 : 1; // White moves up, Black moves down
    int startRow = (color == Color::WHITE) ? 6 : 1;
    
    // Move forward one square
    Position oneForward(position.row + direction, position.col);
    if (oneForward.isValid() && board.isEmpty(oneForward)) {
        moves.push_back(oneForward);
        
        // Move forward two squares from starting position
        if (position.row == startRow) {
            Position twoForward(position.row + 2 * direction, position.col);
            if (twoForward.isValid() && board.isEmpty(twoForward)) {
                moves.push_back(twoForward);
            }
        }
    }
    
    // Capture diagonally
    Position leftCapture(position.row + direction, position.col - 1);
    if (leftCapture.isValid() && board.hasEnemyPiece(leftCapture, color)) {
        moves.push_back(leftCapture);
    }
    
    Position rightCapture(position.row + direction, position.col + 1);
    if (rightCapture.isValid() && board.hasEnemyPiece(rightCapture, color)) {
        moves.push_back(rightCapture);
    }
    
    return moves;
}

std::unique_ptr<Piece> Pawn::clone() const {
    auto cloned = std::make_unique<Pawn>(color, position);
    cloned->setHasMoved(hasMoved);
    return cloned;
}
