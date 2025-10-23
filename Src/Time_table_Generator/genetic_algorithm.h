#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include "timetable.h"

class GeneticAlgorithm {
public:
    int populationSize;
    int generations;
    double mutationRate;

    GeneticAlgorithm(int pop, int gen, double mut)
        : populationSize(pop), generations(gen), mutationRate(mut) {}

    Timetable run(const Timetable& base, bool verbose = true);

private:
    Timetable crossover(const Timetable& p1, const Timetable& p2);
    void mutate(Timetable& t);
};

#endif
