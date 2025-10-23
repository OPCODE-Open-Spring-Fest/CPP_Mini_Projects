cmake_minimum_required(VERSION 3.10)
project(ChessGameAI)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(chess_game
    main.cpp
    Piece.cpp
    Board.cpp
    Player.cpp
    Game.cpp
)

# Enable warnings
if(MSVC)
    target_compile_options(chess_game PRIVATE /W4)
else()
    target_compile_options(chess_game PRIVATE -Wall -Wextra -pedantic)
endif()
