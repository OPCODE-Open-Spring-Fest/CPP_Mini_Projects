#include <iostream>
#include <string>
#include <stdexcept> 
#include "Calculator.h"
using namespace std; 

int main() {
    Calculator calc;
    cout << "Simple C++ Calculator\n";

    while (true) { 
        double a, b;
        char op;

        cout << "\nEnter an operator (+ - * /) or type 'q' to quit: ";
        if (!(cin >> op) || op == 'q') {
            break; 
        }
        
        if (op != '+' && op != '-' && op != '*' && op != '/') {
            cout << "Invalid operator. Please use +, -, *, or /. Try again.\n";
            continue; 
        }

        cout << "Enter first number: ";
        if (!(cin >> a)) {
            cout << "Invalid number input. Quitting.\n";
            break; 
        }

        cout << "Enter second number: ";
        if (!(cin >> b)) {
            cout << "Invalid number input. Quitting.\n";
            break;
        }

        try {
            double result = 0.0;
            switch (op) {
                case '+': result = calc.add(a, b); break;
                case '-': result = calc.subtract(a, b); break;
                case '*': result = calc.multiply(a, b); break;
                case '/': result = calc.divide(a, b); break;
            }
            cout << "Result: " << a << " " << op << " " << b << " = " << result << "\n";
        } catch (const exception &ex) {
            cout << "Error during calculation: " << ex.what() << "\n";
        }
    }
    cout << "Thanks for using the Calculator!\n";
    return 0;
}
