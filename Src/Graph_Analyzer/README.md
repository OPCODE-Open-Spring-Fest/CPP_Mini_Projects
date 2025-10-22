# C++ Graph Analyzer

## Overview
This project is a **Graph Analyzer implemented entirely in C++**. It allows users to **analyze a given graph and extract useful information** about its structure and properties.

The system is designed using **Object-Oriented Programming (OOP)** principles, making the code **modular, maintainable, and easy to extend**.

---

## Features
- Input a graph with vertices and edges (directed or undirected, weighted or unweighted)
- Analyze the graph to determine:
  - Whether the graph contains a **cycle**
  - **Minimum distance** between any two nodes
  - **Connectivity** of the graph
  - Degree of each vertex
  - **Number of connected components**
  - **Graph density**
  - **Graph diameter** (longest shortest path)
  - **Graph radius** (minimum eccentricity)
  - **Eccentricity, centrality, and reachability** of nodes
- Console-based display of analysis results
- Implements **algorithmic techniques** for efficient graph computation

---

## Requirements
- **C++ Implementation**: Entirely written in C++
- **Object-Oriented Design**: Uses classes like `Graph`, `Vertex`, `Edge`, `Analyzer`, etc.
- **Clean Code Practices**:  
  - Meaningful variable and function names  
  - Proper input validation and error handling  
  - Well-documented methods and classes  

---

## Functionality
- Users can provide a graph via adjacency list or edge list
- Analyzer computes key properties of the graph:
  - Detects **cycles**
  - Calculates **shortest paths** between nodes
  - Checks for **graph connectivity**
  - Computes **vertex degrees, graph density, diameter, radius, and number of components**
  - Determines **eccentricity, centrality, and reachability** of nodes
- Efficient algorithms ensure **quick analysis for large graphs**

---




## How to Run
1. Clone the repository:

```bash
git clone https://github.com/<your-username>/GraphAnalyzer.git
cd GraphAnalyzer
