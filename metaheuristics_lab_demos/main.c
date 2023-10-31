#include "../data/r2_r_example_functions.h"
#include "../sa/sa_r2_r_basic.h"
#include <stdio.h>

void* _ten_cones(const void* arg) {
    static double output = 2137;
    output = ten_cones(((r2_cart_t*)arg)->x, ((r2_cart_t*)arg)->y);
    return &output;
}

void ten_cones_demo() {
    r2_cart_t guess;
    guess.x = 0;
    guess.y = 0;
    //guess.x = -293.8926; 
    //guess.y = 404.5085;
    //guess.x = 3;
    //guess.y = 4;

    sa_config_t saConfig;
    saConfig.guess = (void*)&guess;
    saConfig.temperature = 1400;
    saConfig.cooldown = 0.9999;
    saConfig.epochs = /*100*/50000;
    saConfig.epoch_iters = /*1*/1000;
    saConfig.mem_mode = WITH_MEMORY;
    saConfig.emode = MAX;

    r2_cart_rect_constraint_t constraint;
    constraint.xMin = -550.0;
    constraint.xMax = 550.0;
    constraint.yMin = -550.0;
    constraint.yMax = 550.0;

    r2_cart_t solution = sa_r2_r_basic_extreme(_ten_cones, saConfig, constraint);
    printf("Solution X: %4.2f\n", solution.x);
    printf("Solution Y: %4.2f\n", solution.y);
    
}

void fakir_sin_demo() {
    //implement demo
    printf("Implement me\n");
}

int main() {
    ten_cones_demo();
    fakir_sin_demo();
    
    return 0;
}