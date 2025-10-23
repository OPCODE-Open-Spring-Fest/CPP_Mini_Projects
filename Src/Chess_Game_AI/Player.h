#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include <string>
#include <utility>

class Player {
protected:
    Color color;
    std::string name;

public:
    Player(Color c, const std::string& n);
    virtual ~Player() = default;

    Color getColor() const { return color; }
    std::string getName() const { return name; }

    // Get move from player (pure virtual - different for human and AI)
    virtual std::pair<Position, Position> getMove(const Board& board) = 0;
};

class HumanPlayer : public Player {
public:
    HumanPlayer(Color c, const std::string& n);
    std::pair<Position, Position> getMove(const Board& board) override;

private:
    Position parsePosition(const std::string& pos);
};

class AIPlayer : public Player {
private:
    int searchDepth;

public:
    AIPlayer(Color c, const std::string& n, int depth = 3);
    std::pair<Position, Position> getMove(const Board& board) override;

private:
    // Minimax algorithm with alpha-beta pruning
    int minimax(Board& board, int depth, int alpha, int beta, bool isMaximizing);
    
    // Evaluate best move
    std::pair<Position, Position> findBestMove(Board& board);
    
    // Helper function to evaluate board position
    int evaluateBoard(const Board& board) const;
};

#endif // PLAYER_H
