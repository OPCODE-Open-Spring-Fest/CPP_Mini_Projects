# C++ Trie-Based Dictionary Application

## Overview
This project is a **Dictionary Application implemented in C++**, using the **Trie (Prefix Tree)** data structure.  
It provides efficient operations for storing, searching, suggesting, and deleting words.

The system follows **Object-Oriented Programming (OOP)** principles to ensure modularity, clarity, and easy scalability.

---

## Features
- Trie-based dictionary implementation
- Supports:
  - Insertion of new words  
  - Auto-suggestion based on prefix  
  - Deletion of existing words  
  - Display of all dictionary words in sorted order  
- Efficient insert, search, and delete operations (O(length of word))
- Clean OOP architecture:
  - `TrieNode`
  - `Trie`
  - `DictionaryApp`

---

## Components

### 1. TrieNode
Represents a single character node in the Trie:
- Stores child pointers
- Indicates whether the node marks the end of a word

### 2. Trie
Handles all dictionary operations:
- Insert  
- Search  
- Delete  
- Auto-suggest  
- Display all words  

### 3. DictionaryApp
Provides the menu-driven interface:
- Takes user input  
- Executes corresponding Trie operations  
- Displays results interactively  

---

## Detailed Feature Description

### Insert Word
Adds a new word to the Trie and ensures no duplicate insertion.

---

### Auto Suggest
Displays all valid dictionary words that begin with a given prefix, using a depth-first traversal of the Trie.

---

### Delete Word
Removes a word safely by:
- Unmarking the end-of-word  
- Recursively cleaning unused nodes  

Maintains proper Trie structure after deletion.

---

### Display All Words
Traverses the entire Trie and prints every stored word in lexicographically sorted order.

---

## Implementation Requirements
- Full C++ implementation  
- Follows OOP design principles  
- Efficient recursive traversal  
- Clean and modular class structure  
- Handles user input gracefully  

---

## Time Complexity

| Operation        | Time Complexity        |
|------------------|-------------------------|
| Insert           | O(n)                    |
| Search           | O(n)                    |
| Delete           | O(n)                    |
| Auto-suggest     | O(n + k)                |
| Display All      | O(total characters)     |

Where **n** is the length of the word and **k** is the number of suggestions.
