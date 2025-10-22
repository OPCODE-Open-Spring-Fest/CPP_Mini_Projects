# C++ Time Table Generator

> **Note:** This project **must use a Genetic Algorithm in C++** to generate optimized timetables.  
> Your implementation **should be able to handle all edge cases**, including multiple conflicting constraints, limited rooms or time slots, and overlapping instructor schedules.  
> Only submissions that meet these criteria will be considered for PR acceptance.

## Overview
This project is a **Time Table Generator implemented entirely in C++**. It allows users to **automatically generate optimized timetables** for classes, labs, or events while avoiding conflicts.

The system is designed using **Object-Oriented Programming (OOP)** principles, making the code **modular, maintainable, and easy to extend**.  

---

## Features
- Generate conflict-free timetables for multiple classes, instructors, and rooms
- Handles constraints such as:
  - Avoiding overlapping classes for the same instructor
  - Avoiding room conflicts
  - Specific time-slot preferences
- Implements a **Genetic Algorithm** to optimize the timetable
- Console-based display of generated timetables
- Ensures the solution **covers all edge cases**, making it robust for real-world scenarios

---

## Requirements
- **C++ Implementation**: Entirely written in C++
- **Object-Oriented Design**: Uses classes like `TimeTable`, `Class`, `Instructor`, `Room`, `GeneticAlgorithm`, etc.
- **Genetic Algorithm**: Required for optimization
- **Clean Code Practices**:  
  - Meaningful variable and function names  
  - Proper input validation and error handling  
  - Well-documented methods and classes  

---

## Functionality
- Users can input:
  - Number of classes and instructors
  - Available rooms
  - Time slots and constraints
- The program **generates optimized timetables** satisfying all constraints
- Ensures **no conflicts** for instructors, rooms, or overlapping sessions
- Uses **algorithmic techniques (Genetic Algorithm)** to find the best solution efficiently
- Designed to **handle edge cases**, such as multiple conflicting constraints or limited resources

---



## How to Run
1. Clone the repository:

```bash
git clone https://github.com/<your-username>/TimeTable_Generator.git
cd TimeTable_Generator
