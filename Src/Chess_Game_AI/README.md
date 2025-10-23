# C++ Chess Game with AI

## Overview
This project is a **Chess Game implemented entirely in C++**. It allows users to **play chess against an AI opponent** in the console, with **all standard chess rules** enforced.

The system is designed using **Object-Oriented Programming (OOP)** principles, making the code **modular, maintainable, and easy to extend**.

---

## Features
- ♟️ Play chess with all types of pieces: King, Queen, Rook, Bishop, Knight, and Pawn
- 🤖 Human vs AI gameplay mode with 3 difficulty levels
- 👥 Two-player mode (Human vs Human)
- 🧠 AI opponent uses **Minimax algorithm with Alpha-Beta pruning** for intelligent move selection
- 🎨 Console-based board display with ASCII representation
- ✅ Validates moves according to chess rules, including:
  - Legal piece movements
  - Check detection
  - Checkmate detection
  - Stalemate detection
- 🎯 Clean, modular, object-oriented design

---

## Project Structure

```
Chess_Game_AI/
├── Piece.h             # Base Piece class and derived piece classes
├── Piece.cpp           # Implementation of piece movement logic
├── Board.h             # Board class for game state management
├── Board.cpp           # Board operations and rule validation
├── Player.h            # Player base class, HumanPlayer, and AIPlayer
├── Player.cpp          # Player input handling and AI algorithm
├── Game.h              # Game controller class
├── Game.cpp            # Game loop and state management
├── main.cpp            # Entry point with menu system
├── CMakeLists.txt      # Build configuration
└── README.md           # This file
```

---

## Requirements
- **C++ Compiler**: g++, clang++, or MSVC with C++14 support or higher
- **CMake**: Version 3.10 or higher (optional, for easier building)
- **Operating System**: Windows, Linux, or macOS

---

## How to Build and Run

### Option 1: Using CMake (Recommended)

```bash
# Navigate to the Chess_Game_AI directory
cd Src/Chess_Game_AI

# Create build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build the project
cmake --build .

# Run the game
./chess_game         # On Linux/macOS
chess_game.exe       # On Windows
```

### Option 2: Manual Compilation

#### On Windows (using g++):
```bash
cd Src\Chess_Game_AI
g++ -std=c++14 -o chess_game.exe main.cpp Piece.cpp Board.cpp Player.cpp Game.cpp
chess_game.exe
```

#### On Linux/macOS:
```bash
cd Src/Chess_Game_AI
g++ -std=c++14 -o chess_game main.cpp Piece.cpp Board.cpp Player.cpp Game.cpp
./chess_game
```

---

## Gameplay

### Starting the Game
1. Run the executable
2. Choose from the main menu:
   - **1**: Play against AI
   - **2**: Two-player mode
   - **3**: Exit

### AI Difficulty Levels
When playing against AI, select difficulty:
- **Easy (Depth 1)**: AI looks 1 move ahead
- **Medium (Depth 2)**: AI looks 2 moves ahead
- **Hard (Depth 3)**: AI looks 3 moves ahead

### Making Moves
- Players input moves using **algebraic notation**
- Format: `[from] [to]` (e.g., `e2 e4`)
- Columns: **a-h** (left to right)
- Rows: **1-8** (bottom to top for white)

### Examples:
- `e2 e4` - Move pawn from e2 to e4
- `g1 f3` - Move knight from g1 to f3
- `e7 e5` - Move pawn from e7 to e5

### Piece Notation
- **White pieces**: K=King, Q=Queen, R=Rook, B=Bishop, N=Knight, P=Pawn
- **Black pieces**: k=king, q=queen, r=rook, b=bishop, n=knight, p=pawn

### Game End Conditions
- **Checkmate**: One player's king is under attack and cannot escape
- **Stalemate**: Current player has no legal moves but is not in check (Draw)

---

## Architecture

### Class Design

#### 1. **Piece Hierarchy**
- `Piece` (Abstract base class)
  - `King`
  - `Queen`
  - `Rook`
  - `Bishop`
  - `Knight`
  - `Pawn`

Each piece implements:
- `getValidMoves()`: Returns all possible moves
- `getValue()`: Returns piece value for AI evaluation
- `clone()`: Creates a copy for board simulation

#### 2. **Board**
- Manages 8×8 game board
- Handles piece placement and movement
- Validates move legality
- Detects check, checkmate, and stalemate
- Provides board evaluation for AI

#### 3. **Player System**
- `Player` (Abstract base class)
  - `HumanPlayer`: Handles user input
  - `AIPlayer`: Implements AI decision-making

#### 4. **Game Controller**
- Manages game flow
- Handles turn alternation
- Displays board state
- Processes win/loss/draw conditions

### AI Algorithm

The AI uses **Minimax algorithm with Alpha-Beta pruning**:

1. **Minimax**: Evaluates all possible moves recursively
2. **Alpha-Beta Pruning**: Optimizes search by eliminating branches that won't affect the final decision
3. **Board Evaluation**: Calculates position value based on:
   - Material count (piece values)
   - King: 10000, Queen: 900, Rook: 500, Bishop: 330, Knight: 320, Pawn: 100

---

## Code Highlights

### Object-Oriented Principles
- **Encapsulation**: Each class manages its own state
- **Inheritance**: Piece hierarchy with polymorphic behavior
- **Abstraction**: Abstract base classes for extensibility
- **Polymorphism**: Virtual functions for piece-specific behavior

### Design Patterns
- **Strategy Pattern**: Different player types (Human/AI)
- **Template Method**: Common game flow with specialized player moves
- **Prototype Pattern**: Piece cloning for board simulation

### Best Practices
- Smart pointers (`std::unique_ptr`) for memory management
- Const correctness throughout
- Input validation and error handling
- Clear naming conventions
- Modular, testable code structure

---

## Future Enhancements

- [ ] Save/Load game state to file
- [ ] En passant and castling special moves
- [ ] Pawn promotion
- [ ] Move history with undo functionality
- [ ] Advanced AI evaluation (position-based scoring)
- [ ] GUI version using a graphics library
- [ ] Network multiplayer support
- [ ] Opening book for AI
- [ ] Time controls

---

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

---

## License

This project is open source and available under the MIT License.

---

## Author

Developed as part of the CPP_Mini_Projects collection.

---

## Acknowledgments

- Chess rules and notation based on FIDE standards
- AI algorithm inspired by classic game theory
- Built with modern C++ best practices
