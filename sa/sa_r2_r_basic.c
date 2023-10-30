#define __USE_MISC
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "sa_r2_r_basic.h"

/*typedef r2_cart_t (*sa_R2RBasic_FuncTDependent)(const r2_cart_t, const double);
typedef double (*sa_R2RBasic_ScalarFunc)(const double, const double);
typedef CMP_RESULT (*sa_R2RBasic_Predicate)(const double, const double);*/

/**
 * @brief Stochastic select of R2 neighbor, using a quadratic distribution 
 * from points contained in the origin's surrounding circle. The circle size is temperature-dependent.
 * @param r2Cart Structure containing the 2-dimensional cartesian coordinates of the origin point
 * @param temperature The temperature control parameter for the selection
 * @returns The R2 neighbor
*/
void* neighbour(const void* r2Cart, const double temperature) {
    r2_cart_t* _r2Cart = (r2_cart_t*)r2Cart;
    //static r2_cart_t* _neighR2Cart;
    //_neighR2Cart = malloc(0x1);
    static r2_cart_t _neighR2Cart;
    static double distance;
    srand((unsigned int)time(NULL));
    distance = 2 * temperature * (double)rand() / RAND_MAX;
    static double dirAngle;
    dirAngle = 2 * M_PI * (double)rand() / RAND_MAX;
    _neighR2Cart.x = _r2Cart->x;
    _neighR2Cart.y = _r2Cart->y;
    _neighR2Cart.x += distance * cos(dirAngle);
    _neighR2Cart.y += distance * sin(dirAngle);
    
    return (void*)&_neighR2Cart;
}

CMP_RESULT comparer(const void* ra, const void* rb) {
    return (*(double*)rb) > *((double*)ra);
}

double metric(const void* ra, const void* rb) {
    return fabs(*(double*)(rb) - *(double*)(ra));
}

/**
 * @brief Run the simulated annealing metaheuristic algorithm for a R2->R function.
 * @param f The R2->R function to be optimized
 * @param config Structure instance containing algorithm's initial conditions and operating modes,
 * and iterations/epochs count
*/
r2_cart_t sa_r2_r_basic_extreme(const saFunc f, const sa_config_t config) {
    sa_domain_config_t saDomainConfig;
    saDomainConfig.neighbour = neighbour;
    sa_codomain_config_t saCodomainConfig;
    saCodomainConfig.comparer = comparer;
    saCodomainConfig.metric = metric;
    return *(r2_cart_t*)sa_extreme(f, config, saDomainConfig, saCodomainConfig);
}