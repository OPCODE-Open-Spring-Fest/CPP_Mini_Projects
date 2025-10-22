# C++ Operating System Process Simulator

## Overview
This project is an **Operating System Process Simulator implemented entirely in C++**. It allows users to **simulate the scheduling and execution of multiple processes** as handled by an operating system.

The system is designed using **Object-Oriented Programming (OOP)** principles, making the code **modular, maintainable, and easy to extend**.

---

## Features
- Simulate multiple processes with attributes like process ID, arrival time, burst time, and priority
- Supports different **CPU scheduling algorithms**, including:
  - First-Come-First-Served (FCFS)
  - Shortest Job Next (SJN/SJF)
  - Priority Scheduling
  - Round Robin (optional)
- Displays process execution order and Gantt charts in console
- Calculates metrics such as:
  - Waiting time
  - Turnaround time
  - Completion time
- Optional: interactive mode to add, modify, or remove processes

---

## Requirements
- **C++ Implementation**: Entirely written in C++
- **Object-Oriented Design**: Uses classes like `Process`, `Scheduler`, `OSSimulator`, etc.
- **Clean Code Practices**:  
  - Meaningful variable and function names  
  - Proper input validation and error handling  
  - Well-documented methods and classes  

---

## Simulation & Metrics
- Users can input processes with their **arrival and burst times**.
- The simulator calculates **execution order** based on the selected scheduling algorithm.
- Provides metrics such as **average waiting time, turnaround time, and CPU utilization**.
- Useful for understanding how operating systems manage process scheduling.

---

## How to Run
1. Clone the repository:

```bash
git clone https://github.com/<your-username>/OSProcessSimulator.git
cd OSProcessSimulator
