#include "sa.h"

/**
 * @brief Run the simulated annealing metaheuristic algorithm for a R2->R function.
 * @param config Structure instance containing algorithm's initial conditions 
 * and iterations/epochs count
 * @param f The R2->R function to be optimized
 * @param emode Used to choose whether the algorithm shoud seek 
 * the minimum or maximum of the f function
*/
double sa_r2_r_basic_extreme(const sa_config_t config,
                       const saFunc f, 
                       const EXTREME_MODE emode);