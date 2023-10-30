#include "../data/r2_r_example_functions.h"
#include "../sa/sa_r2_r_basic.h"
#include <stdio.h>

void* _ten_cones(const void* arg) {
    static double output;
    output = ten_cones(((r2_cart_t*)arg)->x, ((r2_cart_t*)arg)->y);
    return &output;
}

void ten_cones_demo() {
    r2_cart_t guess;
    guess.x = 0;
    guess.y = 0;

    sa_config_t saConfig;
    saConfig.guess = (void*)&guess;
    saConfig.temperature = 1400;
    saConfig.cooldown = 0.999;
    saConfig.epochs = 100;
    saConfig.epoch_iters = 1;
    saConfig.mem_mode = WITH_MEMORY;
    saConfig.emode = MAX;

    r2_cart_t solution = sa_r2_r_basic_extreme(_ten_cones, saConfig);
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