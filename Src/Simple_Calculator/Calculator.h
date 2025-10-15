#pragma once
#include <stdexcept>

class Calculator {
public:
    // Adds two numbers
    double add(double a, double b) const;

    // Subtracts b from a
    double subtract(double a, double b) const;

    // Multiplies two numbers
    double multiply(double a, double b) const;

    // Divides a by b; throws invalid_argument on division by zero
    double divide(double a, double b) const;
};
