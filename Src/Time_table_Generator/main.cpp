#include "timetable.h"
#include "genetic_algorithm.h"
#include "utils.h"
#include <iostream>
using namespace std;

int main() {
    srand(time(nullptr));
    cout << "\n=== C++ Timetable Generator (Genetic Algorithm) ===\n";

    Timetable base = getUserInput();

    int totalRequiredSessions = 0;
    for (auto &c : base.courses) totalRequiredSessions += c.sessionsPerWeek;
    int totalAvailableSlots = base.numDays * base.slotsPerDay * base.rooms.size();

    if (totalRequiredSessions > totalAvailableSlots) {
        cerr << "\n❌ Scheduling impossible: Total required sessions (" 
             << totalRequiredSessions << ") exceed total available slots (" 
             << totalAvailableSlots << ").\n";
        cerr << "Try reducing sessions or increasing rooms/days/slots.\n";
        return 1;
    }

    GeneticAlgorithm ga(30, 100, 0.1);
    Timetable optimized = ga.run(base, true);

    cout << "\nFinal Best Timetable (Conflicts: " << optimized.computeConflicts() << ")\n";
    optimized.print();
}
