#!/bin/bash
# Chess Game Compilation Script for Linux/macOS

echo "========================================"
echo "Chess Game AI - Build Script"
echo "========================================"
echo

# Check for g++ compiler
if command -v g++ &> /dev/null; then
    echo "Found g++ compiler"
    echo "Compiling with g++..."
    g++ -std=c++14 -Wall -Wextra -o chess_game main.cpp Piece.cpp Board.cpp Player.cpp Game.cpp
    
    if [ $? -eq 0 ]; then
        echo
        echo "========================================"
        echo "Build successful!"
        echo "Run ./chess_game to play"
        echo "========================================"
        chmod +x chess_game
        exit 0
    else
        echo "Build failed!"
        exit 1
    fi
fi

# Check for clang++
if command -v clang++ &> /dev/null; then
    echo "Found clang++ compiler"
    echo "Compiling with clang++..."
    clang++ -std=c++14 -Wall -Wextra -o chess_game main.cpp Piece.cpp Board.cpp Player.cpp Game.cpp
    
    if [ $? -eq 0 ]; then
        echo
        echo "========================================"
        echo "Build successful!"
        echo "Run ./chess_game to play"
        echo "========================================"
        chmod +x chess_game
        exit 0
    else
        echo "Build failed!"
        exit 1
    fi
fi

echo "========================================"
echo "ERROR: No C++ compiler found!"
echo "========================================"
echo
echo "Please install one of the following:"
echo "Ubuntu/Debian: sudo apt-get install g++"
echo "Fedora/RHEL:   sudo dnf install gcc-c++"
echo "macOS:         xcode-select --install"
echo
exit 1
