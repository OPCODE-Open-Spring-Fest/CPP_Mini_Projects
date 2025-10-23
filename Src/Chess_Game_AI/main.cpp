#include "Game.h"
#include <iostream>
#include <limits>

void displayMenu() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "         CHESS GAME MAIN MENU           \n";
    std::cout << "========================================\n";
    std::cout << "1. Play against AI\n";
    std::cout << "2. Two player mode\n";
    std::cout << "3. Exit\n";
    std::cout << "========================================\n";
    std::cout << "Enter your choice: ";
}

int main() {
    bool running = true;
    
    while (running) {
        displayMenu();
        
        int choice;
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please try again.\n";
            continue;
        }
        
        switch (choice) {
            case 1: {
                Game game;
                game.initialize(true); // Play against AI
                game.play();
                break;
            }
            case 2: {
                Game game;
                game.initialize(false); // Two player mode
                game.play();
                break;
            }
            case 3:
                std::cout << "\nThank you for playing! Goodbye.\n";
                running = false;
                break;
            default:
                std::cout << "Invalid choice. Please select 1, 2, or 3.\n";
                break;
        }
        
        if (running && choice != 3) {
            std::cout << "\nPress Enter to return to main menu...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
    }
    
    return 0;
}
