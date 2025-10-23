#include "timetable.h"
#include "genetic_algorithm.h"
#include "utils.h"

int main() {
    srand(time(nullptr));
    cout << "\n=== C++ Timetable Generator (Genetic Algorithm) ===\n";
    Timetable base = getUserInput();

    GeneticAlgorithm ga(30, 100, 0.1); // pop size, generations, mutation rate
    Timetable optimized = ga.run(base, true);

    cout << "\nFinal Best Timetable (Conflicts: " << optimized.computeConflicts() << ")\n";
    optimized.print();
}
