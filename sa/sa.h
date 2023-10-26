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
typedef CMP_RESULT (*saPredicate)(const void*, const void*);
typedef struct {
    void* guess;
    double temperature;
    double cooldown;
    unsigned int epoch_iters;
    unsigned int epochs;

} sa_config_t;

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
                 const saScalarFunc metric);