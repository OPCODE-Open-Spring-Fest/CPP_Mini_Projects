#!/bin/bash
echo "Compiling Lecture 18 C++ Music Player System..."

# Compile the main program
g++ -std=c++11 -I. -o music_player main.cpp

if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo "Running the Music Player application..."
    echo
    ./music_player
else
    echo "Compilation failed!"
    echo "Please check for any syntax errors."
fi
