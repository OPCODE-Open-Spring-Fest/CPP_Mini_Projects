# C++ Simple Calculator

## Overview
This project is a **Simple Calculator implemented in C++**. It performs **basic arithmetic operations** such as addition, subtraction, multiplication, and division.  

The program is designed using **Object-Oriented Programming (OOP)** principles, making the code modular, maintainable, and easy to understand for beginners.

---

## Features
- Perform **addition, subtraction, multiplication, and division**
- Handle invalid inputs (like division by zero)
- Clear, console-based interface
- Optionally, can be extended to support more operations

---

## Requirements
- **C++ Implementation**: Entirely written in C++
- **Object-Oriented Design**: Uses classes like `Calculator` to handle operations
- **Clean Code Practices**:  
  - Meaningful variable and function names  
  - Proper input validation  
  - Clear and readable code  

---

## Build & Run

Files added:
- `Calculator.h` - Calculator class declaration
- `Calculator.cpp` - Calculator class implementation
- `main.cpp` - Interactive console program

Build (requires g++ and C++17):

```powershell
g++ -std=c++17 -Wall -Wextra -O2 main.cpp Calculator.cpp -o calculator.exe
```

Run:

```powershell
.\calculator.exe
```

Usage:
- Enter a number, an operator (+, -, *, /), and a second number. Type `q` at any prompt to quit.

