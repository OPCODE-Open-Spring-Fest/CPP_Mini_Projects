#include "genetic_algorithm.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

Timetable GeneticAlgorithm::crossover(const Timetable& p1, const Timetable& p2) {
    Timetable child = p1;
    size_t half = p1.sessions.size() / 2;
    for (size_t i = half; i < p1.sessions.size(); ++i)
        child.sessions[i] = p2.sessions[i];
    return child;
}

void GeneticAlgorithm::mutate(Timetable& t) {
    if (t.rooms.empty() || t.slotsPerDay <= 0 || t.numDays <= 0) return;

    for (auto &s : t.sessions) {
        if ((rand() / (double)RAND_MAX) < mutationRate) {
            s.day = rand() % t.numDays;
            s.slot = rand() % t.slotsPerDay;
            s.room = t.rooms[rand() % t.rooms.size()].name;
        }
    }
}

Timetable GeneticAlgorithm::run(const Timetable& base, bool verbose) {
    if (base.rooms.empty() || base.slotsPerDay <= 0 || base.numDays <= 0) return base;

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
             [](const Timetable &a, const Timetable &b){ return a.computeConflicts() < b.computeConflicts(); });

        if (population[0].computeConflicts() < bestFitness) {
            best = population[0];
            bestFitness = best.computeConflicts();
        }

        if (bestFitness == 0) break;

        vector<Timetable> newPop;
        for (int i = 0; i < populationSize/2; ++i) {
            Timetable c1 = crossover(population[i], population[rand() % populationSize]);
            Timetable c2 = crossover(population[rand() % populationSize], population[i]);
            mutate(c1);
            mutate(c2);
            newPop.push_back(c1);
            newPop.push_back(c2);
        }
        population = newPop;

        if (verbose && gen % 10 == 0)
            cout << "Generation " << gen << " | Best Conflicts: " << bestFitness << "\n";
    }

    return best;
}
