# C++ Tinder-Like App

## Overview
This project is a **Tinder-like dating app simulation implemented entirely in C++**. It allows users to **create profiles, swipe on other profiles, and match with compatible users**.

The system is designed using **Object-Oriented Programming (OOP)** principles, making the code **modular, maintainable, and easy to extend**.  

> **File Handling:** The app uses file streams to **save and load profiles, matches, and chat history**, ensuring data persists between sessions.

---

## Features
- Create user profiles with attributes such as name, age, gender, interests, and location
- Swipe functionality to **like or pass on profiles**
- **Match users** based on mutual interest and compatibility
- Store and display **matches and chat history**
- Console-based interface for simplicity
- Implements **algorithmic matching logic** to suggest potential matches efficiently
- Input validation to ensure consistent profile information
- **File handling** to persist user data and matches across sessions

---

## Requirements
- **C++ Implementation**: Entirely written in C++
- **Object-Oriented Design**: Uses classes like `User`, `Profile`, `MatchEngine`, `Chat`, etc.
- **File Handling**: Uses `fstream` to read and write profile, match, and chat data
- **Clean Code Practices**:  
  - Meaningful variable and function names  
  - Proper input validation and error handling  
  - Well-documented methods and classes  

---

## Functionality
- Users can:
  - Create or edit their profile
  - Browse other profiles
  - Swipe left (pass) or swipe right (like)
  - See matched users and chat with them
- Matching logic:
  - Uses **algorithmic evaluation** of profile compatibility
  - Suggests potential matches based on mutual likes and shared interests
- **Data persists** between sessions through file handling
- Console interface displays **profile details, matches, and interactions**

---



## How to Run
1. Clone the repository:

```bash
git clone https://github.com/<your-username>/Tinder_Like_App.git
cd Tinder_Like_App
