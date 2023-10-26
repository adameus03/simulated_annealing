#include "sa.h"

/**
 * @brief Run the simulated annealing metaheuristic algorithm for a R2->R function.
 * @param f The R2->R function to be optimized
 * @param config Structure instance containing algorithm's initial conditions and operating modes,
 * and iterations/epochs count
*/
double sa_r2_r_basic_extreme(const saFunc f, const sa_config_t config);