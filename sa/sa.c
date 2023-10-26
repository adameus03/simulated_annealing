#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sa.h"

typedef enum { WORSE, STAY } SA_DECISION;

/**
 * @brief Decides whether to accept a worse candidate or stay with the current solution.
 * The decision is made in the random manner, but using the Boltzmann formula.
 * @param solutionMeasure Optimized function's value for the current solution
 * @param candidateMeasure Optimized function's value for the solution candidate
 * @param temperature Represents the current temperature value in the SA algorithm
 * @param metric Represents the metric used to calculate "distance" 
 * between the solution space objects. For real numbers, you can use |a-b|
 * @returns WORSE or STAY according to the random selection
*/

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


/**
 * @brief The core general function used to run the simulated annealing metaheuristic algorithm.
 * @param config Structure instance containing algorithm's initial conditions 
 * and iterations/epochs count
 * @param f The function to be optimized
 * @param emode Used to choose whether the algorithm shoud seek 
 * the minimum or maximum of the f function
 * @param neighbour A function that is able to select a neighbouring solution space object
 * in a stochastic way, dependent on the current temperature value
 * @param comparer A comparing function that is able to generate a CMP_RESULT comparision result 
 * based on the given argument solution space objects
 * @param metric A metric function that is able to give the algoritm an idea
 * of how far the 2 given objects are from each other in the solution object space
*/
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
