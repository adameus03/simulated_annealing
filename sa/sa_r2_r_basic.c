#include <stdlib.h>
#include "sa_r2_r_basic.h"

typedef struct {
    double x;
    double y;
} r2_cart_t;

r2_cart_t neighbour(r2_cart_t r2Cart) {
    //{{implement}}
}

CMP_RESULT comparer(double ra, double rb) {
    return rb > ra;
}

double metric(double ra, double rb) {
    return abs(rb - ra);
}

double sa_r2_r_basic_extreme(const sa_config_t config,
                       const saFunc f, 
                       const EXTREME_MODE emode) {
    
}