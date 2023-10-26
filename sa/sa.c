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
 * @returns WORSE or STAY according to the stochastic Boltzmann selection 
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
 * @param f The function to be optimized
 * @param config Structure instance containing algorithm's initial conditions and operation modes
 * and iterations/epochs count
 * @param domainConfig Structure instance containing f function's domain configuration for the algorithm
 * @param codomainConfig Structure instance containing f function's codomain configuration for the algorithm
 * @returns The best or the final solution from domain space, depending on the algorithm's memory configuration
*/
void* sa_extreme(const saFunc f,
                 const sa_config_t config,
                 const sa_domain_config_t domainConfig,
                 const sa_codomain_config_t codomainConfig) {
    void* solution = config.guess;
    double temperature = config.temperature;
    void* solutionMeasure = f(solution);

    void* bestSolution = solution;
    void* bestSolutionMeasure = solutionMeasure;

    for (unsigned int epoch = 0; epoch < config.epochs; epoch++) {
        for (unsigned int iter = 0; iter < config.epoch_iters; iter++) {
            static void* candidate;
            candidate = domainConfig.neighbour(solution, temperature);
            static void* candidateMeasure;
            candidateMeasure = f(candidate);

            static CMP_RESULT comparerResult;
            comparerResult = codomainConfig.comparer(candidateMeasure, solutionMeasure);
            if ((config.emode == MAX && comparerResult == LEFT)
                || (config.emode == MIN && comparerResult == RIGHT)
                || sa_decide(solutionMeasure, candidateMeasure, temperature, codomainConfig.metric)
            ) {
                solution = candidate;
                solutionMeasure = candidateMeasure;
                if (config.mem_mode == WITH_MEMORY
                    && codomainConfig.comparer(solutionMeasure, bestSolutionMeasure) == LEFT) {
                    bestSolution = solution;
                    bestSolutionMeasure = solutionMeasure;
                }
            }
        }
        temperature *= config.cooldown;
    }
    switch (config.mem_mode) {
        case WITH_MEMORY:
            return bestSolution;
        case NO_MEMORY:
            return solution;
        default:
            return NULL;
    }
}
