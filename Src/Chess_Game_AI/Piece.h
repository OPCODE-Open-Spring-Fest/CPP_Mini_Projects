#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <memory>

enum class PieceType {
    KING,
    QUEEN,
    ROOK,
    BISHOP,
    KNIGHT,
    PAWN,
    NONE
};

enum class Color {
    WHITE,
    BLACK,
    NONE
};

struct Position {
    int row;
    int col;
    
    Position(int r = 0, int c = 0) : row(r), col(c) {}
    
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
    
    bool isValid() const {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }
};

class Board; // Forward declaration

class Piece {
protected:
    Color color;
    PieceType type;
    Position position;
    bool hasMoved;

public:
    Piece(Color c, PieceType t, Position pos);
    virtual ~Piece() = default;

    // Getters
    Color getColor() const { return color; }
    PieceType getType() const { return type; }
    Position getPosition() const { return position; }
    bool getHasMoved() const { return hasMoved; }

    // Setters
    void setPosition(const Position& pos) { position = pos; }
    void setHasMoved(bool moved) { hasMoved = moved; }

    // Pure virtual function for valid moves
    virtual std::vector<Position> getValidMoves(const Board& board) const = 0;

    // Virtual function to get piece symbol
    virtual char getSymbol() const = 0;

    // Get piece value for AI evaluation
    virtual int getValue() const = 0;

    // Clone method for board simulation
    virtual std::unique_ptr<Piece> clone() const = 0;
};

class King : public Piece {
public:
    King(Color c, Position pos);
    std::vector<Position> getValidMoves(const Board& board) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'K' : 'k'; }
    int getValue() const override { return 10000; }
    std::unique_ptr<Piece> clone() const override;
};

class Queen : public Piece {
public:
    Queen(Color c, Position pos);
    std::vector<Position> getValidMoves(const Board& board) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'Q' : 'q'; }
    int getValue() const override { return 900; }
    std::unique_ptr<Piece> clone() const override;
};

class Rook : public Piece {
public:
    Rook(Color c, Position pos);
    std::vector<Position> getValidMoves(const Board& board) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'R' : 'r'; }
    int getValue() const override { return 500; }
    std::unique_ptr<Piece> clone() const override;
};

class Bishop : public Piece {
public:
    Bishop(Color c, Position pos);
    std::vector<Position> getValidMoves(const Board& board) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'B' : 'b'; }
    int getValue() const override { return 330; }
    std::unique_ptr<Piece> clone() const override;
};

class Knight : public Piece {
public:
    Knight(Color c, Position pos);
    std::vector<Position> getValidMoves(const Board& board) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'N' : 'n'; }
    int getValue() const override { return 320; }
    std::unique_ptr<Piece> clone() const override;
};

class Pawn : public Piece {
public:
    Pawn(Color c, Position pos);
    std::vector<Position> getValidMoves(const Board& board) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'P' : 'p'; }
    int getValue() const override { return 100; }
    std::unique_ptr<Piece> clone() const override;
};

#endif // PIECE_H
