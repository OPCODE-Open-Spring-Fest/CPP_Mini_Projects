#include <iostream>
#include <string>
#include <limits>   
#include <stdexcept>  

#include "CurrencyConverter.h"

double getValidAmount() {
    double amount;
    while (true) {
        std::cout << "Enter amount to convert: ";
        if (std::cin >> amount) {
            if (amount > 0) {
                return amount;
            } else {
                std::cout << "Please enter a positive amount." << std::endl;
            }
        } else {
            // This handles non-numeric input
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear(); // Clear the error flag
            // Discard the invalid input from the buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

// Helper function to get a valid currency code from the user
std::string getValidCurrency(const std::string& prompt, CurrencyConverter& converter) {
    std::string code;
    while (true) {
        std::cout << prompt;
        std::cin >> code;
        
        // Convert input to uppercase to be safe
        for (auto &c : code) c = toupper(c);

        if (converter.isCurrencySupported(code)) {
            return code; // Valid currency
        } else {
            std::cout << "Invalid or unsupported currency code. Please try again." << std::endl;
            converter.printAvailableCurrencies();
        }
    }
}


int main() {
    // 1. Create an instance of our converter.
    // This calls the constructor and loads the rates.
    CurrencyConverter converter;

    std::cout << "Welcome to the C++ Currency Converter!" << std::endl;

    // 2. Main application loop
    while (true) {
        std::cout << "\n------------------------------------" << std::endl;
        converter.printAvailableCurrencies();

        // 3. Get all user inputs
        std::string from = getValidCurrency("Convert FROM (e.g., USD): ", converter);
        std::string to = getValidCurrency("Convert TO (e.g., EUR): ", converter);
        double amount = getValidAmount();

        // 4. Perform conversion and handle errors
        try {
            double result = converter.convert(amount, from, to);
            
            // Print the result
            std::cout << "\n--- Result ---" << std::endl;
            std::cout << amount << " " << from << " = " << result << " " << to << std::endl;
        
        } catch (const std::exception& e) {
            // This will catch errors we threw in our .convert() function
            std::cout << e.what() << std::endl;
        }

        // 5. Ask to go again
        std::cout << "\nDo you want to perform another conversion? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice != 'y' && choice != 'Y') {
            break; // Exit the while loop
        }
    }

    std::cout << "Thank you for using the converter. Goodbye!" << std::endl;
    return 0;
}