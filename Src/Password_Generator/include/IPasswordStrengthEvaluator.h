#pragma once
#include <string>

enum class PasswordStrength { Weak, Medium, Strong };

struct IPasswordStrengthEvaluator {
    virtual ~IPasswordStrengthEvaluator() = default;
    virtual double entropyBits(const std::string& password) const = 0;
    virtual PasswordStrength classify(double entropyBits) const = 0;
    virtual const char* toString(PasswordStrength s) const = 0;
};
