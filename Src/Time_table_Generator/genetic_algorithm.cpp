#include "genetic_algorithm.h"

Timetable GeneticAlgorithm::crossover(const Timetable& p1, const Timetable& p2) {
    Timetable child = p1;
    int half = p1.sessions.size() / 2;
    for (size_t i = half; i < p1.sessions.size(); ++i)
        child.sessions[i] = p2.sessions[i];
    return child;
}

void GeneticAlgorithm::mutate(Timetable& t) {
    int n = t.sessions.size();
    for (int i = 0; i < n; ++i) {
        if ((rand() / (double)RAND_MAX) < mutationRate) {
            t.sessions[i].day = rand() % t.numDays;
            t.sessions[i].slot = rand() % t.slotsPerDay;
            t.sessions[i].room = t.rooms[rand() % t.rooms.size()].name;
        }
    }
}

Timetable GeneticAlgorithm::run(const Timetable& base, bool verbose) {
    vector<Timetable> population;
    for (int i = 0; i < populationSize; ++i) {
        Timetable t = base;
        t.generateRandom();
        population.push_back(t);
    }

    Timetable best = population[0];
    int bestFitness = best.computeConflicts();

    for (int gen = 0; gen < generations; ++gen) {
        sort(population.begin(), population.end(),
             [](const Timetable &a, const Timetable &b) {
                 return a.computeConflicts() < b.computeConflicts();
             });

        if (population[0].computeConflicts() < bestFitness) {
            best = population[0];
            bestFitness = best.computeConflicts();
        }

        if (bestFitness == 0) break; // Perfect timetable found

        vector<Timetable> newPop;
        for (int i = 0; i < populationSize / 2; ++i) {
            Timetable child1 = crossover(population[i], population[rand() % populationSize]);
            Timetable child2 = crossover(population[rand() % populationSize], population[i]);
            mutate(child1);
            mutate(child2);
            newPop.push_back(child1);
            newPop.push_back(child2);
        }
        population = newPop;

        if (verbose && gen % 10 == 0)
            cout << "Generation " << gen << " | Best Conflicts: " << bestFitness << "\n";
    }

    return best;
}
