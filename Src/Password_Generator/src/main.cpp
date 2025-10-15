#include "PasswordGenerator.h"
#include "PasswordStrengthEvaluator.h"
#include "Random.h"
#include "UserPreferences.h"
#include <iostream>
#include <limits>
#include <string>
#include <vector>

static std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

static bool yesNo(const std::string& prompt, bool def = true) {
    while (true) {
        std::string s = readLine(prompt + (def ? " [Y/n]: " : " [y/N]: "));
        if (s.empty()) return def;
        for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (s == "y" || s == "yes") return true;
        if (s == "n" || s == "no") return false;
        std::cout << "Please answer y or n.\n";
    }
}

static std::size_t readSizeT(const std::string& prompt, std::size_t def, std::size_t minV = 1, std::size_t maxV = 1024) {
    while (true) {
        std::string s = readLine(prompt + " [" + std::to_string(def) + "]: ");
        if (s.empty()) return def;
        try {
            std::size_t v = std::stoull(s);
            if (v < minV || v > maxV) {
                std::cout << "Enter a value between " << minV << " and " << maxV << ".\n";
                continue;
            }
            return v;
        } catch (...) {
            std::cout << "Invalid number.\n";
        }
    }
}

static UserPreferences readCustomPreferences() {
    UserPreferences prefs;
    prefs.length = readSizeT("Password length", prefs.length, 1, 2048);
    prefs.includeLowercase = yesNo("Include lowercase letters?", true);
    prefs.includeUppercase = yesNo("Include uppercase letters?", true);
    prefs.includeDigits    = yesNo("Include digits?", true);
    prefs.includeSymbols   = yesNo("Include symbols?", false);
    prefs.requireEachSelectedType = yesNo("Require at least one of each selected type?", true);
    prefs.count = readSizeT("How many passwords to generate", 1, 1, 10000);
    return prefs;
}

int main() {
    std::cout << "C++ Password Generator\n";
    std::cout << "1) Preset (weak/medium/strong)\n2) Custom\n";
    std::string choice = readLine("Choose option [1/2]: ");
    UserPreferences prefs;

    if (choice == "1") {
        std::string preset = readLine("Preset [weak/medium/strong]: ");
        for (auto& c : preset) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (preset == "weak") prefs = UserPreferences::Weak();
        else if (preset == "medium") prefs = UserPreferences::Medium();
        else if (preset == "strong") prefs = UserPreferences::Strong();
        else {
            std::cout << "Unknown preset. Using 'strong'.\n";
            prefs = UserPreferences::Strong();
        }
        prefs.count = readSizeT("How many passwords to generate", 1, 1, 10000);
    } else {
        prefs = readCustomPreferences();
    }

    MTRandom rng;
    PasswordGenerator generator{rng};
    EntropyPasswordStrengthEvaluator evaluator;

    try {
        auto passwords = generator.generateMany(prefs);
        for (std::size_t i = 0; i < passwords.size(); ++i) {
            const auto& p = passwords[i];
            double e = evaluator.entropyBits(p);
            auto cls = evaluator.classify(e);
            std::cout << "[" << (i + 1) << "] " << p << "  (entropy: " << static_cast<int>(e)
                      << " bits, strength: " << evaluator.toString(cls) << ")\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
