@echo off
echo Compiling C++ Music Player System...

REM Compile the main program
g++ -std=c++11 -I. -o music_player main.cpp

if %errorlevel% equ 0 (
    echo Compilation successful!
    echo Running the Music Player application...
    echo.
    music_player.exe
) else (
    echo Compilation failed!
    echo Please check for any syntax errors.
)

pause
