#ifndef UTILS_H
#define UTILS_H

#include "timetable.h"
#include <regex>
#include <iostream>
#include <string>

constexpr int MAX_DAYS = 7;
constexpr int MAX_SLOTS = 24;
constexpr int MAX_ROOMS = 50;
constexpr int MAX_COURSES = 100;

// Robust numeric input
inline int getSafeInt(const std::string &prompt, int minVal, int maxVal) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);

        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);

        if (line.empty()) {
            std::cout << "Input cannot be empty! Please enter a number.\n";
            continue;
        }

        bool allDigits = true;
        for (char c : line) {
            if (!isdigit(c)) { allDigits = false; break; }
        }
        if (!allDigits) {
            std::cout << "Invalid input! Enter a numeric integer.\n";
            continue;
        }

        try {
            int val = std::stoi(line);
            if (val < minVal || val > maxVal) {
                std::cout << "Invalid input! Enter integer between " << minVal << " and " << maxVal << ".\n";
                continue;
            }
            return val;
        } catch (...) {
            std::cout << "Invalid input! Could not convert to number.\n";
        }
    }
}

// Non-empty string input
inline std::string getNonEmptyString(const std::string &prompt) {
    std::string s;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, s);
        if (!s.empty()) return s;
        std::cout << "Input cannot be empty! Please enter a valid value.\n";
    }
}

// Validate slot label (HH:MM-HH:MM or single uppercase letter)
inline bool isValidSlotLabel(const std::string &s) {
    std::regex timePattern(R"(\d{1,2}:\d{2}-\d{1,2}:\d{2})");
    std::regex letterPattern(R"([A-Z])");
    return std::regex_match(s, timePattern) || std::regex_match(s, letterPattern);
}

inline std::string getSlotLabel(int i) {
    std::string label;
    while (true) {
        std::cout << "Slot " << i + 1 << " label (HH:MM-HH:MM or A-Z): ";
        std::getline(std::cin, label);
        if (label.empty()) label = "Slot " + std::to_string(i + 1);
        if (isValidSlotLabel(label)) break;
        std::cout << "Invalid format! Use HH:MM-HH:MM (09:00-10:00) or single letter (A-Z).\n";
    }
    return label;
}

inline Timetable getUserInput() {
    int days = getSafeInt("Enter number of days in week (1-7): ", 1, MAX_DAYS);
    int slots = getSafeInt("Enter slots per day (1-24): ", 1, MAX_SLOTS);

    std::vector<std::string> slotLabels(slots);
    std::cout << "\nEnter label or time for each slot:\n";
    for (int i = 0; i < slots; ++i) slotLabels[i] = getSlotLabel(i);

    int numRooms = getSafeInt("\nEnter number of rooms (1-50): ", 1, MAX_ROOMS);
    std::vector<Room> rooms(numRooms);
    for (int i = 0; i < numRooms; ++i) rooms[i].name = getNonEmptyString("Room " + std::to_string(i + 1) + " name: ");

    int numCourses = getSafeInt("\nEnter number of courses (1-100): ", 1, MAX_COURSES);
    std::vector<Course> courses(numCourses);

    int totalAvailableSlots = days * slots * numRooms;

    for (int i = 0; i < numCourses; ++i) {
        std::cout << "\nCourse " << i + 1 << " details:\n";
        courses[i].name = getNonEmptyString("Course name: ");
        courses[i].instructor = getNonEmptyString("Instructor name: ");
        courses[i].sessionsPerWeek = getSafeInt("Sessions per week: ", 1, totalAvailableSlots);

        int prefCount = getSafeInt("Number of preferred slots (0 for none): ", 0, slots);
        courses[i].allowedSlots.clear();
        for (int j = 0; j < prefCount; ++j) {
            int slot = getSafeInt("Preferred slot index (1-" + std::to_string(slots) + "): ", 1, slots);
            courses[i].allowedSlots.push_back(slot - 1);
        }
    }

    return Timetable(days, slots, rooms, courses, slotLabels);
}

#endif
