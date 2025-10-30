@echo off
echo Compiling C++ Discount Coupon Application...

REM Compile the main program
g++ -std=c++11 -o discount_coupon DiscountCoupon.cpp

if %errorlevel% equ 0 (
    echo Compilation successful!
    echo Running the Discount Coupon application...
    echo.
    discount_coupon.exe
) else (
    echo Compilation failed!
    echo Please check for any syntax errors.
)

pause
