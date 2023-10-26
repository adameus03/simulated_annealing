#define __USE_MISC
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "sa_r2_r_basic.h"

typedef struct {
    double x;
    double y;
} r2_cart_t;
/*typedef r2_cart_t (*sa_R2RBasic_FuncTDependent)(const r2_cart_t, const double);
typedef double (*sa_R2RBasic_ScalarFunc)(const double, const double);
typedef CMP_RESULT (*sa_R2RBasic_Predicate)(const double, const double);*/

/**
 * @brief Run the simulated annealing metaheuristic algorithm for a R2->R function.
 * @param config Structure instance containing algorithm's initial conditions 
 * and iterations/epochs count
 * @param f The R2->R function to be optimized
 * @param emode Used to choose whether the algorithm shoud seek 
 * the minimum or maximum of the f function
*/
void* neighbour(const void* r2Cart, const double temperature) {
    //{{implement}}
    r2_cart_t* _r2Cart = (r2_cart_t*)r2Cart;
    static r2_cart_t* _neighR2Cart;
    _neighR2Cart = malloc(0x1);

    static double distance;
    srand((unsigned int)time(NULL));
    distance = 2 * temperature * (double)rand() / RAND_MAX;
    static double dirAngle;
    dirAngle = 2 * M_PI * (double)rand() / RAND_MAX;
    _neighR2Cart->x = _r2Cart->x;
    _neighR2Cart->y = _r2Cart->y;
    _neighR2Cart->x += distance * cos(dirAngle);
    _neighR2Cart->y += distance * sin(dirAngle);
    
    return (void*)_neighR2Cart;
}

CMP_RESULT comparer(const void* ra, const void* rb) {
    return (*(double*)rb) > *((double*)ra);
}

double metric(const void* ra, const void* rb) {
    return fabs(*(double*)(rb) - *(double*)(ra));
}

double sa_r2_r_basic_extreme(const sa_config_t config,
                       const saFunc f, 
                       const EXTREME_MODE emode) {

    return *(double*)sa_extreme(config, 
                                f, 
                                emode, 
                                neighbour, 
                                comparer, 
                                metric);
}