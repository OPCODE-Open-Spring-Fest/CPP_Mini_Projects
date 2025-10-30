@echo off
REM Chess Game Compilation Script for Windows

echo ========================================
echo Chess Game AI - Build Script
echo ========================================
echo.

REM Check for g++ compiler
where g++ >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo Found g++ compiler
    echo Compiling with g++...
    g++ -std=c++14 -Wall -Wextra -o chess_game.exe main.cpp Piece.cpp Board.cpp Player.cpp Game.cpp
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo ========================================
        echo Build successful!
        echo Run chess_game.exe to play
        echo ========================================
        pause
        exit /b 0
    ) else (
        echo Build failed!
        pause
        exit /b 1
    )
)

REM Check for MSVC compiler
where cl >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo Found MSVC compiler
    echo Compiling with MSVC...
    cl /EHsc /std:c++14 /Fe:chess_game.exe main.cpp Piece.cpp Board.cpp Player.cpp Game.cpp
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo ========================================
        echo Build successful!
        echo Run chess_game.exe to play
        echo ========================================
        pause
        exit /b 0
    ) else (
        echo Build failed!
        pause
        exit /b 1
    )
)

REM Check for clang++
where clang++ >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo Found clang++ compiler
    echo Compiling with clang++...
    clang++ -std=c++14 -o chess_game.exe main.cpp Piece.cpp Board.cpp Player.cpp Game.cpp
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo ========================================
        echo Build successful!
        echo Run chess_game.exe to play
        echo ========================================
        pause
        exit /b 0
    ) else (
        echo Build failed!
        pause
        exit /b 1
    )
)

echo ========================================
echo ERROR: No C++ compiler found!
echo ========================================
echo.
echo Please install one of the following:
echo 1. MinGW-w64 (includes g++)
echo    Download from: https://www.mingw-w64.org/
echo.
echo 2. Microsoft Visual Studio (includes MSVC)
echo    Download from: https://visualstudio.microsoft.com/
echo.
echo 3. LLVM/Clang
echo    Download from: https://releases.llvm.org/
echo.
pause
exit /b 1
