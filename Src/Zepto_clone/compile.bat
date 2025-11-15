@echo off
echo Compiling C++ Zepto Clone Application...

REM Compile the main program
g++ -std=c++11 -o zepto_clone ZeptoClone.cpp

if %errorlevel% equ 0 (
    echo Compilation successful!
    echo Running the Zepto Clone application...
    echo.
    zepto_clone.exe
) else (
    echo Compilation failed!
    echo Please check for any syntax errors.
)

pause

