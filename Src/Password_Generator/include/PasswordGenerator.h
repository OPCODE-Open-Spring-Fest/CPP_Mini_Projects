#pragma once
#include "IRandom.h"
#include "UserPreferences.h"
#include <string>
#include <vector>

class PasswordGenerator {
public:
    explicit PasswordGenerator(IRandom& rng);

    std::string generate(const UserPreferences& prefs);
    std::vector<std::string> generateMany(const UserPreferences& prefs);

private:
    IRandom& rng_;

    std::string buildAlphabet(const UserPreferences& prefs) const;
    void ensureValid(const UserPreferences& prefs) const;
    char randomFrom(const std::string& alphabet);
    void fisherYatesShuffle(std::string& s);
    std::string generateWithEachType(const UserPreferences& prefs, const std::string& alphabet);
};
