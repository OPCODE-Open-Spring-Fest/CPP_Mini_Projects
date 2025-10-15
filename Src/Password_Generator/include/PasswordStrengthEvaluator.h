#pragma once
#include "IPasswordStrengthEvaluator.h"

class EntropyPasswordStrengthEvaluator final : public IPasswordStrengthEvaluator {
public:
    double entropyBits(const std::string& password) const override;
    PasswordStrength classify(double entropyBits) const override;
    const char* toString(PasswordStrength s) const override;
};
