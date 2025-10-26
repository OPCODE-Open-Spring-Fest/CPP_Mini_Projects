@echo off
echo Compiling C++ Food Delivery App...

REM Compile the main program
g++ -std=c++11 -I. -o tomato_app main.cpp

if %errorlevel% equ 0 (
    echo Compilation successful!
    echo Running the application...
    echo.
    tomato_app.exe
) else (
    echo Compilation failed!
    echo Please check for any syntax errors.
)

pause
