@echo off
echo Compiling C++ Chain of Responsibility Application...

REM Compile the main program
g++ -std=c++11 -o cor COR.cpp

if %errorlevel% equ 0 (
    echo Compilation successful!
    echo Running the Chain of Responsibility application...
    echo.
    cor.exe
) else (
    echo Compilation failed!
    echo Please check for any syntax errors.
)

pause

