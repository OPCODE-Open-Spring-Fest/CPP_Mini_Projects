# C++ Currency Converter

## Overview
This project is a **Currency Converter application implemented in C++**. It allows users to **convert amounts between different currencies** based on predefined or updatable exchange rates.  

The program is designed using **Object-Oriented Programming (OOP)** principles, making the code **modular, maintainable, and easy to extend**.

---

## Features
- Convert between multiple currencies (USD, EUR, INR, GBP, etc.)
- User-friendly console interface
- Optional: update exchange rates
- Handle invalid inputs gracefully
- Optional: display conversion history

---

## Requirements
- **C++ Implementation**: Entirely written in C++
- **Object-Oriented Design**: Uses classes like `CurrencyConverter`, `Currency`, etc.
- **Clean Code Practices**:  
  - Meaningful variable and function names  
  - Proper input validation and error handling  
  - Clear and readable methods  

---

# How to Compile and Run

## Prerequisites
You will need a C++ compiler installed on your system, such as **g++** (part of the **MinGW-w64** toolset on Windows) or **Clang**.  
Make sure it is added to your system's **PATH**.

---

## Steps

1. **Open your terminal or command prompt.**

2. **Navigate to the project directory** where the `.cpp` files are located:

   ```bash
   cd path/to/Currency_Converter
  ```

3. **Compile and Run**
  # Compile the source code
  Run the following command in your terminal:

  ```bash
  g++ main.cpp CurrencyConverter.cpp -o currency_converter -std=c++11
  ```

This will compile all the necessary files and create a single executable program named  
`currency_converter` (or `currency_converter.exe` on Windows).

---

  # Run the Application

  **On Windows (PowerShell/CMD):**
  ```bash
  .\currency_converter.exe
  ```

  **On macOS or Linux:**
  ```bash
  ./currency_converter
  ```

Follow the on-screen prompts to use the converter.