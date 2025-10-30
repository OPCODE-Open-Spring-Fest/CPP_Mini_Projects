@echo off
echo C++ Payment Gateway Application...

REM Compile the main program
g++ -std=c++11 -o payment_gateway PaymentGatewayApplication.cpp

if %errorlevel% equ 0 (
    echo Compilation successful!
    echo Running the Payment Gateway application...
    echo.
    payment_gateway.exe
) else (
    echo Compilation failed!
    echo Please check for any syntax errors.
)

pause
