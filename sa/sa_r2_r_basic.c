#define __USE_MISC
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "sa_r2_r_basic.h"
#include <stdio.h> //debug

/*typedef r2_cart_t (*sa_R2RBasic_FuncTDependent)(const r2_cart_t, const double);
typedef double (*sa_R2RBasic_ScalarFunc)(const double, const double);
typedef CMP_RESULT (*sa_R2RBasic_Predicate)(const double, const double);*/

r2_cart_rect_constraint_t _constraint(const r2_cart_rect_constraint_t* constraintPtr) {
    static r2_cart_rect_constraint_t constraint;
    if (constraintPtr == NULL) {
        return constraint;
    }
    else {
        constraint = *constraintPtr;
        return constraint;
    }
}

/**
 * @brief Stochastic select of R2 neighbor, using a quadratic distribution 
 * from points contained in the origin's surrounding circle. The circle size is temperature-dependent.
 * @param r2Cart Structure containing the 2-dimensional cartesian coordinates of the origin point
 * @param temperature The temperature control parameter for the selection
 * @returns The R2 neighbor
*/
void* neighbour(const void* r2Cart, const double temperature) {
    r2_cart_t* _r2Cart = (r2_cart_t*)r2Cart;
    //printf("Temperature: %4.2f; X: %4.2f; Y: %4.2f\n", temperature, _r2Cart->x, _r2Cart->y);
    static r2_cart_t _neighR2Cart;
    static double distance;
    srand((unsigned int)time(NULL));
    distance = 2/*10*//*0.5*//*0.39*/ * temperature * (double)rand() / RAND_MAX;
    static double dirAngle;
    dirAngle = 2 * M_PI * (double)rand() / RAND_MAX;
    _neighR2Cart.x = _r2Cart->x;
    _neighR2Cart.y = _r2Cart->y;
    _neighR2Cart.x += distance * cos(dirAngle);
    _neighR2Cart.y += distance * sin(dirAngle);

    static r2_cart_rect_constraint_t constraint;
    constraint = _constraint(NULL);
    if (_neighR2Cart.x < constraint.xMin) {
        _neighR2Cart.x += constraint.xMax - constraint.xMin;
    }
    else if (_neighR2Cart.x > constraint.xMax) {
        _neighR2Cart.x -= constraint.xMax - constraint.xMin;
    }
    if (_neighR2Cart.y < constraint.yMin) {
        _neighR2Cart.y += constraint.yMax - constraint.yMin;
    }
    else if (_neighR2Cart.y > constraint.yMax) {
        _neighR2Cart.y -= constraint.yMax - constraint.yMin;
    }
    
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
 * @param constraint Structure instance containing rectangular bounds for the optimized function domain
 * @returns The best or the final solution from the constrained R2 space, depending on the algorithm's memory configuration
 * @note It is possible to overload this function in order to enable different constraint types
*/
r2_cart_t sa_r2_r_basic_extreme(const saFunc f, 
                                const sa_config_t config, 
                                const r2_cart_rect_constraint_t constraint) {
    sa_domain_config_t saDomainConfig;
    saDomainConfig.neighbour = neighbour;
    sa_codomain_config_t saCodomainConfig;
    saCodomainConfig.comparer = comparer;
    saCodomainConfig.metric = metric;

    ///<test>
    //-293.8926 404.5085
    r2_cart_t temp;
    temp.x = -293.8926;
    temp.y = 404.5085;
    double val = *(double*)f(&temp);
    printf("f(-293.8926, 404.5085) = %2.4f\n", val);
    ///</test>

    sa_type(r2_cart_t, double);
    _constraint(&constraint);
    return *(r2_cart_t*)sa_extreme(f, config, saDomainConfig, saCodomainConfig);
}