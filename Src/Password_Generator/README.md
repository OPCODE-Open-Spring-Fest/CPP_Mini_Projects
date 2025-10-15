# C++ Password Generator

## Overview
This project is a **Password Generator application written in C++**, following **Object-Oriented Programming (OOP)** principles. It generates **strong, random passwords** based on user-defined preferences such as length, inclusion of numbers, symbols, and uppercase/lowercase letters.  

The project is designed to be **modular, maintainable, and easy to extend**, following clean code practices and recommended OOP principles.

---

## Features
- Generate secure and random passwords
- Customize password length
- Include/exclude numbers, symbols, uppercase, and lowercase letters
- Optionally generate multiple passwords at once
- Optional password strength levels: weak, medium, strong
- Follows clean OOP structure with separate classes

---

## Requirements
- **C++ Implementation**: Entirely written in C++
- **Object-Oriented Design**: Uses classes like `PasswordGenerator`, `UserPreferences`, etc.
- **SOLID Principles**: Ensures code is modular, maintainable, and extendable
- **Production-Level Code**:  
  - Meaningful variable names  
  - Proper error handling and input validation  
  - Clear comments and documentation

---

## Installation
1. Clone the repository:  
   ```bash
   git clone https://github.com/your-username/your-repo.git
   ```
2. Navigate to the Password Generator project directory:
   ```bash
   cd your-repo/Src/Password_Generator
   ```
3. Build with CMake:
   ```bash
   mkdir -p build && cd build
   cmake ..
   cmake --build .
   ```
4. Run:
   ```bash
   ./password_generator
   ```

Alternative: build without CMake (ensure you are in Src/Password_Generator)
```bash
g++ -std=cpp17 -Iinclude src/*.cpp -o password_generator
./password_generator
```

## Usage
The app runs in interactive CLI mode.

- Option 1 (Preset): Choose weak, medium, or strong, then how many passwords to generate.
- Option 2 (Custom): Choose length, inclusion of lowercase/uppercase/digits/symbols, whether to require at least one of each selected type, and how many passwords.

Example session
```
C++ Password Generator
1) Preset (weak/medium/strong)
2) Custom
Choose option [1/2]: 1
Preset [weak/medium/strong]: strong
How many passwords to generate [1]: 3
[1] 2Gz!tq@K3wqM)h^L  (entropy: 95 bits, strength: strong)
[2] Lz5{nZV8pQ%g=J7!  (entropy: 95 bits, strength: strong)
[3] 7Y@kR`mT2s&Qh9)C  (entropy: 95 bits, strength: strong)
```

Options explained
- length: number of characters in each password.
- includeLowercase/includeUppercase/includeDigits/includeSymbols: toggle character sets.
- requireEachSelectedType: if enabled, guarantees at least one character from each selected set and shuffles the result.
- count: how many passwords to output.

## Design overview (OOP + SOLID)
- PasswordGenerator: single responsibility to generate passwords; depends on abstractions.
- IRandom + MTRandom: randomness abstraction and mt19937-based implementation (Dependency Inversion).
- UserPreferences: clean container for user options and presets (Weak/Medium/Strong).
- IPasswordStrengthEvaluator + EntropyPasswordStrengthEvaluator: pluggable strength evaluation.
- CharacterSets: centralized immutable character sets.
- Dependency Injection: Generator receives IRandom; evaluator is independent and reusable.

## Notes
- Requires a C++17-compatible compiler and CMake 3.15+ (if using CMake).
- Input validated with helpful error messages (e.g., length must be > 0, at least one character set required).
