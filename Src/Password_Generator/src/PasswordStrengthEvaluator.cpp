#include "PasswordStrengthEvaluator.h"
#include "CharacterSets.h"
#include <cmath>
#include <string_view>

static bool containsAny(std::string_view s, std::string_view bag) {
    for (char c : s) {
        for (char b : bag) {
            if (c == b) return true;
        }
    }
    return false;
}

double EntropyPasswordStrengthEvaluator::entropyBits(const std::string& password) const {
    using namespace std::literals;
    std::string_view pw{password};
    double alphabetSize = 0.0;
    if (containsAny(pw, charset::LOWER))  alphabetSize += std::char_traits<char>::length(charset::LOWER);
    if (containsAny(pw, charset::UPPER))  alphabetSize += std::char_traits<char>::length(charset::UPPER);
    if (containsAny(pw, charset::DIGITS)) alphabetSize += std::char_traits<char>::length(charset::DIGITS);
    if (containsAny(pw, charset::SYMBOLS))alphabetSize += std::char_traits<char>::length(charset::SYMBOLS);

    if (alphabetSize <= 1.0) return 0.0;
    return static_cast<double>(password.size()) * std::log2(alphabetSize);
}

PasswordStrength EntropyPasswordStrengthEvaluator::classify(double e) const {
    if (e < 36.0) return PasswordStrength::Weak;
    if (e < 60.0) return PasswordStrength::Medium;
    return PasswordStrength::Strong;
}

const char* EntropyPasswordStrengthEvaluator::toString(PasswordStrength s) const {
    switch (s) {
        case PasswordStrength::Weak:   return "weak";
        case PasswordStrength::Medium: return "medium";
        case PasswordStrength::Strong: return "strong";
    }
    return "unknown";
}
