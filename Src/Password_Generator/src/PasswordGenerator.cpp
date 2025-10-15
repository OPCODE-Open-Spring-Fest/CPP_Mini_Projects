#include "PasswordGenerator.h"
#include "CharacterSets.h"
#include <stdexcept>

PasswordGenerator::PasswordGenerator(IRandom& rng) : rng_{rng} {}

void PasswordGenerator::ensureValid(const UserPreferences& prefs) const {
    if (prefs.length == 0) throw std::invalid_argument("Password length must be > 0");
    if (!prefs.includeLowercase && !prefs.includeUppercase && !prefs.includeDigits && !prefs.includeSymbols) {
        throw std::invalid_argument("At least one character set must be enabled");
    }
    std::size_t types = (prefs.includeLowercase?1:0) + (prefs.includeUppercase?1:0) +
                        (prefs.includeDigits?1:0) + (prefs.includeSymbols?1:0);
    if (prefs.requireEachSelectedType && prefs.length < types) {
        throw std::invalid_argument("Length too short to include at least one of each selected type");
    }
}

std::string PasswordGenerator::buildAlphabet(const UserPreferences& prefs) const {
    std::string alphabet;
    if (prefs.includeLowercase) alphabet += charset::LOWER;
    if (prefs.includeUppercase) alphabet += charset::UPPER;
    if (prefs.includeDigits)    alphabet += charset::DIGITS;
    if (prefs.includeSymbols)   alphabet += charset::SYMBOLS;
    return alphabet;
}

char PasswordGenerator::randomFrom(const std::string& alphabet) {
    return alphabet[rng_.nextIndex(alphabet.size())];
}

void PasswordGenerator::fisherYatesShuffle(std::string& s) {
    if (s.size() <= 1) return;
    for (std::size_t i = s.size() - 1; i > 0; --i) {
        std::size_t j = rng_.nextIndex(i + 1);
        std::swap(s[i], s[j]);
    }
}

std::string PasswordGenerator::generateWithEachType(const UserPreferences& prefs, const std::string& alphabet) {
    std::string pw;
    pw.reserve(prefs.length);

    // Ensure at least one char from each selected set
    if (prefs.includeLowercase) pw.push_back(randomFrom(charset::LOWER));
    if (prefs.includeUppercase) pw.push_back(randomFrom(charset::UPPER));
    if (prefs.includeDigits)    pw.push_back(randomFrom(charset::DIGITS));
    if (prefs.includeSymbols)   pw.push_back(randomFrom(charset::SYMBOLS));

    // Fill remaining with random chars from the combined alphabet
    while (pw.size() < prefs.length) {
        pw.push_back(randomFrom(alphabet));
    }

    fisherYatesShuffle(pw);
    return pw;
}

std::string PasswordGenerator::generate(const UserPreferences& prefs) {
    ensureValid(prefs);
    const std::string alphabet = buildAlphabet(prefs);
    if (prefs.requireEachSelectedType) return generateWithEachType(prefs, alphabet);

    std::string pw;
    pw.reserve(prefs.length);
    for (std::size_t i = 0; i < prefs.length; ++i) {
        pw.push_back(randomFrom(alphabet));
    }
    return pw;
}

std::vector<std::string> PasswordGenerator::generateMany(const UserPreferences& prefs) {
    if (prefs.count == 0) throw std::invalid_argument("Count must be > 0");
    std::vector<std::string> out;
    out.reserve(prefs.count);
    for (std::size_t i = 0; i < prefs.count; ++i) {
        out.push_back(generate(prefs));
    }
    return out;
}
