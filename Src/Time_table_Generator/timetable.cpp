#include "timetable.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

void Timetable::generateRandom() {
    sessions.clear();
    srand(time(nullptr));
    for (auto &course : courses) {
        for (int i = 0; i < course.sessionsPerWeek; ++i) {
            Session session;
            session.courseName = course.name;
            session.instructor = course.instructor;
            session.room = rooms[rand() % rooms.size()].name;
            session.day = rand() % numDays;
            session.slot = rand() % slotsPerDay;
            sessions.push_back(session);
        }
    }
}

int Timetable::computeConflicts() const {
    int conflicts = 0;

    // Room & instructor conflicts
    for (size_t i = 0; i < sessions.size(); ++i) {
        for (size_t j = i + 1; j < sessions.size(); ++j) {
            const Session &a = sessions[i];
            const Session &b = sessions[j];
            if (a.day == b.day && a.slot == b.slot) {
                if (a.room == b.room) conflicts++;            // Room conflict
                if (a.instructor == b.instructor) conflicts++; // Instructor conflict
            }
        }
    }

    // Preferred time-slot conflicts
    for (auto &s : sessions) {
        auto it = find_if(courses.begin(), courses.end(),
                          [&](const Course &c){ return c.name == s.courseName; });
        if (it != courses.end() && !it->allowedSlots.empty()) {
            if (find(it->allowedSlots.begin(), it->allowedSlots.end(), s.slot) == it->allowedSlots.end()) {
                conflicts++; // Penalize for being outside preferred slots
            }
        }
    }

    return conflicts;
}

void Timetable::print() const {
    vector<Session> sorted = sessions;
    sort(sorted.begin(), sorted.end(), [](const Session &a, const Session &b) {
        if (a.day != b.day) return a.day < b.day;
        return a.slot < b.slot;
    });

    cout << "\n===== Optimized Timetable =====\n";
    for (auto &s : sorted) {
        string slotLabel = (s.slot >= 0 && s.slot < (int)slotLabels.size() && !slotLabels[s.slot].empty())
                            ? slotLabels[s.slot] 
                            : ("Slot " + to_string(s.slot + 1));

        cout << "Day " << s.day + 1 << ", " << slotLabel
             << " | " << s.courseName
             << " | Instructor: " << s.instructor
             << " | Room: " << s.room << "\n";
    }
    cout << "===============================\n";
}
