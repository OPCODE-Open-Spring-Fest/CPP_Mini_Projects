#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <bits/stdc++.h>
using namespace std;

struct Course {
    string name;
    string instructor;
    int sessionsPerWeek;
    vector<int> allowedSlots; // optional preferred slots (0-based)
};

struct Room {
    string name;
};

struct Session {
    string courseName;
    string instructor;
    string room;
    int day;
    int slot;
};

class Timetable {
public:
    int numDays;
    int slotsPerDay;
    vector<string> slotLabels;
    vector<Room> rooms;
    vector<Course> courses;
    vector<Session> sessions;

    Timetable() {}
    Timetable(int d, int s, const vector<Room>& r, const vector<Course>& c,
              const vector<string>& labels)
        : numDays(d), slotsPerDay(s), rooms(r), courses(c), slotLabels(labels) {}

    void generateRandom();
    int computeConflicts() const;
    void print() const;
};

#endif
