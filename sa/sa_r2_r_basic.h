#include "sa.h"

typedef struct {
    double x;
    double y;
} r2_cart_t;

typedef struct {
    double xMin;
    double xMax;
    double yMin;
    double yMax;
} r2_cart_rect_constraint_t;

/**
 * @brief Run the simulated annealing metaheuristic algorithm for a R2->R function.
 * @param f The R2->R function to be optimized
 * @param config Structure instance containing algorithm's initial conditions and operating modes,
 * and iterations/epochs count
 * @param constraint Structure instance containing rectangular bounds for the optimized function domain
 * @returns The best or the final solution from the constrained R2 space, depending on the algorithm's memory configuration
*/
r2_cart_t sa_r2_r_basic_extreme(const saFunc f,
                                const sa_config_t config,
                                const r2_cart_rect_constraint_t constraint);