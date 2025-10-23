# Chess Game AI - Installation and Compilation Guide

## Prerequisites

Before compiling the Chess Game, you need to have a C++ compiler installed on your system.

---

## Windows

### Option 1: Install MinGW-w64 (Recommended for Windows)

1. Download MinGW-w64 from: https://www.mingw-w64.org/downloads/
   - Or use the installer: https://github.com/msys2/msys2-installer/releases

2. Install and add to PATH:
   - During installation, note the installation directory
   - Add `C:\mingw-w64\bin` (or your installation path) to system PATH
   - Open Command Prompt and verify: `g++ --version`

3. Compile the game:
   ```cmd
   cd Src\Chess_Game_AI
   build.bat
   ```
   
   Or manually:
   ```cmd
   g++ -std=c++14 -o chess_game.exe main.cpp Piece.cpp Board.cpp Player.cpp Game.cpp
   ```

4. Run the game:
   ```cmd
   chess_game.exe
   ```

### Option 2: Microsoft Visual Studio

1. Download Visual Studio Community (free):
   https://visualstudio.microsoft.com/downloads/

2. During installation, select "Desktop development with C++"

3. Open "Developer Command Prompt for VS"

4. Compile:
   ```cmd
   cd Src\Chess_Game_AI
   cl /EHsc /std:c++14 /Fe:chess_game.exe main.cpp Piece.cpp Board.cpp Player.cpp Game.cpp
   ```

5. Run:
   ```cmd
   chess_game.exe
   ```

### Option 3: Using CMake

1. Install CMake: https://cmake.org/download/

2. Build:
   ```cmd
   cd Src\Chess_Game_AI
   mkdir build
   cd build
   cmake ..
   cmake --build .
   chess_game.exe
   ```

---

## Linux

### Ubuntu/Debian

1. Install g++:
   ```bash
   sudo apt-get update
   sudo apt-get install g++ build-essential
   ```

2. Compile:
   ```bash
   cd Src/Chess_Game_AI
   chmod +x build.sh
   ./build.sh
   ```
   
   Or manually:
   ```bash
   g++ -std=c++14 -o chess_game main.cpp Piece.cpp Board.cpp Player.cpp Game.cpp
   ```

3. Run:
   ```bash
   ./chess_game
   ```

### Fedora/RHEL/CentOS

1. Install g++:
   ```bash
   sudo dnf install gcc-c++
   ```

2. Follow the same compilation steps as Ubuntu

### Arch Linux

1. Install g++:
   ```bash
   sudo pacman -S gcc
   ```

2. Follow the same compilation steps as Ubuntu

---

## macOS

### Using Xcode Command Line Tools

1. Install Xcode tools:
   ```bash
   xcode-select --install
   ```

2. Compile:
   ```bash
   cd Src/Chess_Game_AI
   chmod +x build.sh
   ./build.sh
   ```
   
   Or manually:
   ```bash
   clang++ -std=c++14 -o chess_game main.cpp Piece.cpp Board.cpp Player.cpp Game.cpp
   ```

3. Run:
   ```bash
   ./chess_game
   ```

### Using Homebrew

1. Install g++ via Homebrew:
   ```bash
   brew install gcc
   ```

2. Follow the same compilation steps

---

## Using CMake (Cross-Platform)

CMake provides a platform-independent build system.

### Install CMake

- **Windows**: Download from https://cmake.org/download/
- **Linux**: `sudo apt-get install cmake` or `sudo dnf install cmake`
- **macOS**: `brew install cmake`

### Build with CMake

```bash
cd Src/Chess_Game_AI
mkdir build
cd build
cmake ..
cmake --build .
```

### Run

- **Windows**: `chess_game.exe`
- **Linux/macOS**: `./chess_game`

---

## Compilation Flags Explained

- `-std=c++14`: Use C++14 standard
- `-Wall`: Enable all warnings
- `-Wextra`: Enable extra warnings
- `-o chess_game`: Output executable name

---

## Troubleshooting

### "g++ not recognized" (Windows)

- Ensure g++ is installed and added to PATH
- Restart Command Prompt/PowerShell after installation

### "Permission denied" (Linux/macOS)

```bash
chmod +x chess_game
```

### "No such file or directory"

- Ensure you're in the correct directory (`Src/Chess_Game_AI`)
- Verify all source files are present

### Compilation Errors

- Check that all `.h` and `.cpp` files are in the same directory
- Ensure compiler supports C++14 or later
- Update compiler if needed

---

## Quick Start (After Compilation)

1. Run the executable
2. Choose game mode:
   - Option 1: Play against AI
   - Option 2: Two-player mode
3. Select AI difficulty (if playing against AI)
4. Enter moves using algebraic notation (e.g., `e2 e4`)
5. Enjoy the game!

---

## Files Required for Compilation

Make sure all these files are present:
- `main.cpp`
- `Piece.h` and `Piece.cpp`
- `Board.h` and `Board.cpp`
- `Player.h` and `Player.cpp`
- `Game.h` and `Game.cpp`

---

## Support

For issues or questions, please refer to the README.md file or submit an issue on the project repository.
