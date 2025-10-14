#include "Calculator.h"
using namespace std;

double Calculator::add(double a, double b) const {
    return a+b;
}

double Calculator::subtract(double a, double b) const {
    return a-b;
}

double Calculator::multiply(double a, double b) const {
    return a*b;
}

double Calculator::divide(double a, double b) const {
    if (b==0.0) {
        throw invalid_argument("Division by zero is undefined");
    }
    return a/b;
}
