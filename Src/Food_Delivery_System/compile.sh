#!/bin/bash
echo "Compiling Lecture 11 C++ Tomato App..."

# Compile the main program
g++ -std=c++11 -I. -o tomato_app main.cpp

if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo "Running the application..."
    echo
    ./tomato_app
else
    echo "Compilation failed!"
    echo "Please check for any syntax errors."
fi
