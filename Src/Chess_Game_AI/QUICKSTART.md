# Chess Game - Quick Reference Guide

## Board Layout

```
    a   b   c   d   e   f   g   h
  +---+---+---+---+---+---+---+---+
8 | r | n | b | q | k | b | n | r | 8  <- Black's back rank
  +---+---+---+---+---+---+---+---+
7 | p | p | p | p | p | p | p | p | 7  <- Black pawns
  +---+---+---+---+---+---+---+---+
6 |   |   |   |   |   |   |   |   | 6
  +---+---+---+---+---+---+---+---+
5 |   |   |   |   |   |   |   |   | 5
  +---+---+---+---+---+---+---+---+
4 |   |   |   |   |   |   |   |   | 4
  +---+---+---+---+---+---+---+---+
3 |   |   |   |   |   |   |   |   | 3
  +---+---+---+---+---+---+---+---+
2 | P | P | P | P | P | P | P | P | 2  <- White pawns
  +---+---+---+---+---+---+---+---+
1 | R | N | B | Q | K | B | N | R | 1  <- White's back rank
  +---+---+---+---+---+---+---+---+
    a   b   c   d   e   f   g   h
```

## Piece Symbols

### White Pieces (Uppercase)
- **K** = King
- **Q** = Queen
- **R** = Rook
- **B** = Bishop
- **N** = Knight (N is used to avoid confusion with King)
- **P** = Pawn

### Black Pieces (Lowercase)
- **k** = king
- **q** = queen
- **r** = rook
- **b** = bishop
- **n** = knight
- **p** = pawn

## Algebraic Notation

### Squares
- **Columns**: a, b, c, d, e, f, g, h (left to right)
- **Rows**: 1, 2, 3, 4, 5, 6, 7, 8 (bottom to top for White)
- Each square is identified by column + row (e.g., e4, a1, h8)

### Move Format
```
[from square] [to square]
```

Examples:
- `e2 e4` - Move piece from e2 to e4
- `g1 f3` - Move piece from g1 to f3
- `d7 d5` - Move piece from d7 to d5

## Common Opening Moves

### White's Opening Moves
1. `e2 e4` - King's Pawn Opening (most popular)
2. `d2 d4` - Queen's Pawn Opening
3. `g1 f3` - King's Knight Opening
4. `c2 c4` - English Opening

### Black's Responses to e2 e4
1. `e7 e5` - King's Pawn (symmetrical)
2. `c7 c5` - Sicilian Defense
3. `e7 e6` - French Defense
4. `c7 c6` - Caro-Kann Defense

## Piece Movement Rules

### King (K/k)
- Moves one square in any direction
- Most important piece (losing it = checkmate)

### Queen (Q/q)
- Moves any number of squares in any direction (horizontal, vertical, diagonal)
- Most powerful piece

### Rook (R/r)
- Moves any number of squares horizontally or vertically
- Cannot move diagonally

### Bishop (B/b)
- Moves any number of squares diagonally
- Each bishop stays on its starting color (light or dark squares)

### Knight (N/n)
- Moves in "L" shape: 2 squares in one direction, 1 square perpendicular
- Only piece that can jump over other pieces

### Pawn (P/p)
- Moves forward one square
- First move: can move forward two squares
- Captures diagonally (one square diagonally forward)
- Cannot move backward

## Special Rules

### Check
- When a king is under attack
- Player must move out of check
- Indicated by "CHECK!" message in the game

### Checkmate
- When a king is in check and cannot escape
- Game over - checking player wins

### Stalemate
- When a player has no legal moves but is not in check
- Game is a draw (tie)

## Sample Game Sequence

```
White's turn: e2 e4     (Move pawn from e2 to e4)
Black's turn: e7 e5     (Move pawn from e7 to e5)
White's turn: g1 f3     (Move knight from g1 to f3)
Black's turn: b8 c6     (Move knight from b8 to c6)
White's turn: f1 c4     (Move bishop from f1 to c4)
Black's turn: f8 c5     (Move bishop from f8 to c5)
...
```

## Tips for Playing

### For Beginners
1. Control the center (e4, e5, d4, d5)
2. Develop pieces (knights and bishops) early
3. Castle to protect your king
4. Don't move the same piece twice in the opening
5. Don't bring queen out too early

### Playing Against AI
- **Easy**: Good for learning
- **Medium**: Balanced challenge
- **Hard**: Requires strategic thinking

### Common Mistakes to Avoid
1. Leaving pieces undefended
2. Ignoring threats to your king
3. Moving without a plan
4. Trading good pieces for weaker ones
5. Not thinking ahead

## Game End Scenarios

### You Win
- Checkmate opponent's king
- Opponent resigns (in two-player mode)

### Draw
- Stalemate (no legal moves, not in check)
- Insufficient material (e.g., king vs king)

### You Lose
- Your king is checkmated
- You resign (in two-player mode)

## Input Examples

### Valid Inputs
- `e2 e4` ✓
- `a7 a5` ✓
- `h1 h3` ✓

### Invalid Inputs
- `e2e4` ✗ (missing space)
- `E2 E4` ✗ (use lowercase)
- `e9 e10` ✗ (invalid squares)
- `z5 a3` ✗ (column 'z' doesn't exist)

## Strategy Tips

### Opening Principles (First 10 moves)
1. Control center squares
2. Develop minor pieces (knights, bishops)
3. Castle early
4. Connect rooks

### Middle Game
1. Look for tactical opportunities
2. Improve piece positions
3. Create threats
4. Defend against opponent's threats

### End Game
1. Activate your king
2. Promote pawns
3. Coordinate pieces
4. Calculate precisely

## Practice Exercises

### Beginner
Try these opening sequences:
1. e2 e4, e7 e5, g1 f3, b8 c6
2. d2 d4, d7 d5, c2 c4, e7 e6

### Intermediate
Practice knight moves:
1. Move knight from g1 to f3
2. Then to e5
3. Then back to f3

---

**Remember**: Chess is a game of strategy and patience. Take your time, think ahead, and enjoy the game!
