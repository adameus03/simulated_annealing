#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum { MAX, MIN } EXTREME_MODE;
typedef enum { LEFT, RIGHT } CMP_RESULT;
typedef enum { WORSE, STAY } SA_DECISION;
typedef void* (*saFunc)(const void*);
typedef void* (*saFuncTDependent)(const void*, const double);
typedef double (*saScalarFunc)(const void*, const void*);
typedef CMP_RESULT (*saPredicate)(void*, void*);
typedef struct {
    void* guess;
    double temperature;
    double cooldown;
    unsigned int epoch_iters;
    unsigned int epochs;

} sa_config_t;

SA_DECISION sa_decide(const void* solutionMeasure, 
                      const void* candidateMeasure, 
                      const double temperature,
                      const saScalarFunc metric) {
    static double probabilityWorse;
    double metricValue = metric(solutionMeasure, candidateMeasure);
    probabilityWorse = exp(-metricValue / temperature);
    srand((unsigned int)time(NULL));
    double randomValue = (double)rand() / RAND_MAX;
    if (randomValue < probabilityWorse) {
        return WORSE;
    }
    return STAY;

}



void* sa_extreme(const sa_config_t config,
                 const saFunc f, 
                 const EXTREME_MODE emode, 
                 const saFuncTDependent neighbour, 
                 const saPredicate comparer,
                 const saScalarFunc metric) {
    void* solution = config.guess;
    double temperature = config.temperature;
    void* solutionMeasure = f(solution);

    for (unsigned int epoch = 0; epoch < config.epochs; epoch++) {
        for (unsigned int iter = 0; iter < config.epoch_iters; iter++) {
            static void* candidate;
            candidate = neighbour(solution, temperature);
            static void* candidateMeasure;
            candidateMeasure = f(candidate);

            static CMP_RESULT comparerResult;
            comparerResult = comparer(candidateMeasure, solutionMeasure);
            if ((emode == MAX && comparerResult == LEFT)
                || (emode == MIN && comparerResult == RIGHT)
                || sa_decide(solutionMeasure, candidateMeasure, temperature, metric)
            ) {
                solution = candidate;
                solutionMeasure = candidateMeasure;
            }
        }
        temperature *= config.cooldown;
    }
    return solution;
}
