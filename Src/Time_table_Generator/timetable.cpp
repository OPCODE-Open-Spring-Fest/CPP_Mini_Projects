#include "timetable.h"
#include <algorithm>
#include <iostream>
using namespace std;

void Timetable::generateRandom() {
    if (rooms.empty() || slotsPerDay <= 0 || numDays <= 0) return;

    sessions.clear();
    srand(time(nullptr));
    for (auto &course : courses) {
        for (int i = 0; i < course.sessionsPerWeek; ++i) {
            Session s;
            s.courseName = course.name;
            s.instructor = course.instructor;
            s.room = rooms[rand() % rooms.size()].name;
            s.day = rand() % numDays;
            s.slot = rand() % slotsPerDay;
            sessions.push_back(s);
        }
    }
}

int Timetable::computeConflicts() const {
    int conflicts = 0;
    for (size_t i = 0; i < sessions.size(); ++i) {
        for (size_t j = i + 1; j < sessions.size(); ++j) {
            const Session &a = sessions[i];
            const Session &b = sessions[j];
            if (a.day == b.day && a.slot == b.slot) {
                if (a.room == b.room) conflicts++;
                if (a.instructor == b.instructor) conflicts++;
            }
        }
    }
    for (auto &s : sessions) {
        auto it = find_if(courses.begin(), courses.end(),
                          [&](const Course &c){ return c.name == s.courseName; });
        if (it != courses.end() && !it->allowedSlots.empty()) {
            if (find(it->allowedSlots.begin(), it->allowedSlots.end(), s.slot) == it->allowedSlots.end())
                conflicts++;
        }
    }
    return conflicts;
}

void Timetable::print() const {
    vector<Session> sorted = sessions;
    sort(sorted.begin(), sorted.end(), [](const Session &a, const Session &b){
        if (a.day != b.day) return a.day < b.day;
        return a.slot < b.slot;
    });

    cout << "\n===== Optimized Timetable =====\n";
    for (auto &s : sorted) {
        string slotLabel = (s.slot >= 0 && s.slot < (int)slotLabels.size() && !slotLabels[s.slot].empty())
                            ? slotLabels[s.slot]
                            : ("Slot " + to_string(s.slot + 1));
        cout << "Day " << s.day+1 << ", " << slotLabel
             << " | " << s.courseName
             << " | Instructor: " << s.instructor
             << " | Room: " << s.room << "\n";
    }
    cout << "===============================\n";
}
