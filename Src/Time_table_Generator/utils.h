#ifndef UTILS_H
#define UTILS_H

#include "timetable.h"

Timetable getUserInput() {
    int days, slots, numRooms, numCourses;
    cout << "Enter number of days in week: ";
    cin >> days;
    cout << "Enter slots per day: ";
    cin >> slots;

    cout << "\nEnter a label or time for each slot (e.g., 9:00-10:00):\n";
    vector<string> slotLabels(slots);
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush buffer
    for (int i = 0; i < slots; ++i) {
        cout << "Slot " << i + 1 << " label: ";
        getline(cin, slotLabels[i]);
        if (slotLabels[i].empty()) slotLabels[i] = "Slot " + to_string(i + 1);
    }

    cout << "\nEnter number of rooms: ";
    cin >> numRooms;
    vector<Room> rooms(numRooms);
    for (int i = 0; i < numRooms; ++i) {
        cout << "Room " << i + 1 << " name: ";
        cin >> rooms[i].name;
    }

    cout << "\nEnter number of courses: ";
    cin >> numCourses;
    vector<Course> courses(numCourses);
    for (int i = 0; i < numCourses; ++i) {
        cout << "\nCourse " << i + 1 << " name: ";
        cin >> courses[i].name;
        cout << "Instructor name: ";
        cin >> courses[i].instructor;
        cout << "Sessions per week: ";
        cin >> courses[i].sessionsPerWeek;

        // Preferred slots
        cout << "Enter number of preferred slots for this course (0 for no preference): ";
        int prefCount;
        cin >> prefCount;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush leftover newline

        courses[i].allowedSlots.clear();
        for (int j = 0; j < prefCount; ++j) {
            int slot;
            cout << "Preferred slot index (1-" << slots << "): ";
            cin >> slot;
            courses[i].allowedSlots.push_back(slot - 1); // 0-based index
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush after the loop

    }

    return Timetable(days, slots, rooms, courses, slotLabels);
}

#endif
