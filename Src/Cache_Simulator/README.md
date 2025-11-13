# C++ Cache Memory Simulator

## Overview
This project is a **Cache Memory Simulator implemented in C++**.  
It emulates how a **CPU cache** interacts with **main memory** and supports multiple **cache replacement algorithms** to analyze cache performance.  

The simulator is designed using **Object-Oriented Programming (OOP)** principles, making the code **modular, maintainable, and easy to extend** for learning or experimentation.

---

## Features
- Simulates real-world **cache memory behavior**
- Supports major cache replacement strategies:
  - **FIFO (First-In First-Out)**
  - **LRU (Least Recently Used)**
  - **LFU (Least Frequently Used)**
  - **Belady’s Optimal Algorithm** *(for theoretical comparison)*
- Allows **custom cache configuration**:
  - Cache size  
  - Block size  
  - Associativity (n-way)
- Reads **variable memory access sequences** from an input file (`cacheData.txt`)
- Displays **cache state after every access**
- Tracks:
  - Cache **hits** and **misses**
  - **Hit/Miss rates**
  - **Average Memory Access Time (AMAT)**

---

## Requirements
- **C++ Implementation**: Entirely written in C++  
- **Object-Oriented Design**: Uses classes like `CacheBlock`, `Cache`, and `ReplacementPolicy`  
- **Clean Code Practices**:  
  - Structured and well-commented code  
  - Modular design with inheritance for replacement policies  
  - Proper statistics and performance tracking  

---
