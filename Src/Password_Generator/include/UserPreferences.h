#pragma once
#include <cstddef>

struct UserPreferences {
    std::size_t length{12};
    bool includeLowercase{true};
    bool includeUppercase{true};
    bool includeDigits{true};
    bool includeSymbols{false};
    std::size_t count{1};
    bool requireEachSelectedType{true};

    static UserPreferences Weak() {
        return UserPreferences{8, true, false, false, false, 1, false};
    }
    static UserPreferences Medium() {
        return UserPreferences{12, true, true, true, false, 1, true};
    }
    static UserPreferences Strong() {
        return UserPreferences{16, true, true, true, true, 1, true};
    }
};
